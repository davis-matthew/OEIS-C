/*
 * A002681  Numerators of coefficients for repeated integration.
 * https://oeis.org/A002681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of ((n+1)/2)*M(n) + (2n+2)*M(n+1), where M(n) =
 *            (2/(2n+1)!) * Integral_{t=0..1} t*Product_{k=1..n}(t^2-k^2) dt =
 *            I(n)/(2n+1)!, I(n) = Integral_{u=0..1} Product_{k=1..n}(u-k^2)
 *            du. So a(n) = numerator of (n+1)/(2n+1)! * ( I(n)/2 +
 *            I(n+1)/((n+1)(2n+3)) )
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define DEF_A2681(T, SFX)                                                     \
static T gcd_##SFX(T x, T y) {                                                \
  if (x < 0) x = -x;                                                          \
  if (y < 0) y = -y;                                                          \
  while (y != 0) { T t = x % y; x = y; y = t; }                               \
  return x;                                                                   \
}                                                                             \
static void red_##SFX(T *p, T *q) {                                           \
  T g;                                                                        \
  if (*q < 0) { *p = -*p; *q = -*q; }                                         \
  g = gcd_##SFX(*p, *q);                                                      \
  if (g != 0 && g != 1) { *p /= g; *q /= g; }                                 \
}                                                                             \
static void integ_##SFX(idx_t n, T *num, T *den) {                            \
  T *c = (T *)calloc((size_t)n + 2, sizeof(T));                               \
  T L, S, g;                                                                  \
  idx_t i, k;                                                                 \
  for (i = 0; i <= n + 1; i++) c[i] = 0;                                      \
  c[0] = 1;                                                                   \
  for (k = 1; k <= n; k++) {                                                  \
    T s = (T)k * (T)k;                                                        \
    for (i = k; i >= 1; i--) c[i] = c[i - 1] - s * c[i];                      \
    c[0] = -s * c[0];                                                         \
  }                                                                           \
  L = 1;                                                                      \
  for (i = 2; i <= n + 1; i++) {                                              \
    T ti = (T)i;                                                              \
    g = gcd_##SFX(L, ti);                                                     \
    L = L / g * ti;                                                           \
  }                                                                           \
  S = 0;                                                                      \
  for (i = 0; i <= n; i++) S += c[i] * (L / (T)(i + 1));                      \
  free(c);                                                                    \
  red_##SFX(&S, &L);                                                          \
  *num = S; *den = L;                                                         \
}                                                                             \
static T calc_##SFX(idx_t n) {                                                \
  T p1, q1, p2, q2, An, Ad, Bn, Bd, g, num, den, F, g1, g2, m, g3;            \
  idx_t i;                                                                    \
  integ_##SFX(n, &p1, &q1);                                                   \
  integ_##SFX(n + 1, &p2, &q2);                                               \
  An = p1; Ad = q1 * 2;                                                       \
  red_##SFX(&An, &Ad);                                                        \
  Bn = p2; Bd = q2 * ((T)n + 1) * ((T)n * 2 + 3);                             \
  red_##SFX(&Bn, &Bd);                                                        \
  g = gcd_##SFX(Ad, Bd);                                                      \
  num = An * (Bd / g) + Bn * (Ad / g);                                        \
  den = Ad / g * Bd;                                                          \
  red_##SFX(&num, &den);                                                      \
  F = 1;                                                                      \
  for (i = 2; i <= 2 * n + 1; i++) F *= (T)i;                                 \
  g2 = gcd_##SFX(num, F);                                                     \
  if (g2 != 0) { num /= g2; F /= g2; }                                        \
  m = (T)n + 1;                                                               \
  g1 = gcd_##SFX(m, den);                                                     \
  m /= g1;                                                                    \
  g3 = gcd_##SFX(m, F);                                                       \
  m /= g3;                                                                    \
  return num * m;                                                             \
}

DEF_A2681(__int128, w)
DEF_A2681(seq_t, s)

seq_t a(idx_t n) {
  if (sizeof(seq_t) <= 8) {
    __int128 r = calc_w(n);
    return (seq_t)(long long)r;
  }
  return calc_s(n);
}
