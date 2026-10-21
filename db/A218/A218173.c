/*
 * A218173  Expansion of f(x^7, x^17) - x^2 * f(x, x^23) in powers of x where
 *          f(,) is Ramanujan's two-variable theta function.
 * https://oeis.org/A218173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (exists integer k: n = 12k^2 + 5k) - (exists integer k: n
 *            = 12k^2 + 11k + 2) Equivalently: let D = 25 + 48n and s =
 *            floor(sqrt(D)). If s*s != D then a(n)=0. If s ≡ 5 or 19 (mod 24)
 *            then a(n)= 1. If s ≡ 11 or 13 (mod 24) then a(n)= -1
 * Range:     i8 n <= 0, i16 n <= 330, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Compute D = 25 + 48*n using seq_t to avoid overflow   */
    seq_t D = (seq_t)48 * (seq_t)n + (seq_t)25;
    seq_t s = floor_sqrt(D);
    if (s * s != D) return (seq_t)0;

    unsigned r = (unsigned)(s % (seq_t)24);    /* s is non‑negative */

    switch (r) {
        case 5:
        case 19:  return (seq_t)1;
        case 11:
        case 13:  return (seq_t)-1;
        default:  return (seq_t)0;
    }
}
