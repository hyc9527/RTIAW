#include "HittableObjectList.h"

// helper type for the visitor #4
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace RTIAW::Render {
HitResult HittableObject::Hit(const Ray &r, float t_min, float t_max) const {
  constexpr HitResult empty_result{{}, {}};

  const auto hitr = std::visit(
      overloaded{
          [&](const Shapes::Sphere &sphere) { return sphere.Hit(r, t_min, t_max); },
      },
      m_shape);

  if (!hitr)
    return empty_result;

  return {hitr, Scatter(r, hitr.value())};
}

std::optional<ScatteringRecord> HittableObject::Scatter(const Ray &r, const HitRecord &rec) const {
  return std::visit(overloaded{
                        [&](const Materials::Lambertian &lambertian) { return lambertian.Scatter(r, rec); },
                        [&](const Materials::Metal &metal) { return metal.Scatter(r, rec); },
                        [&](const Materials::Dielectric &dielectric) { return dielectric.Scatter(r, rec); },
                    },
                    m_material);
}
} // namespace RTIAW::Render
