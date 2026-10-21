/*
 * A205988  Expansion of f(x^1, x^9) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A205988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if there exists an integer k with 5*k^2 + 4*k = n,
 *            otherwise a(n) = 0. This follows from the expansion f(x,x^9) =
 *            Σ_{k∈ℤ} x^(5*k^2 + 4*k)
 * Range:     u8 n <= 2, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* D = 20*n + 16  (must be a perfect square) */
    seq_t D = (seq_t)n;
    D = D * 20 + 16;

    seq_t sqrtD = floor_sqrt(D);
    if (sqrtD * sqrtD != D) return (seq_t)0;

    /* For a perfect square D, 5k^2+4k = n iff sqrtD % 10 is 4 or 6. */
    seq_t mod10 = sqrtD % 10;
    if (mod10 == 4 || mod10 == 6) return (seq_t)1;
    return (seq_t)0;
}
