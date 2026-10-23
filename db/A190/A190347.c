/*
 * A190347  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(5/3), t=sqrt(3/5).
 * https://oeis.org/A190347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*sqrt(5/3)⌋ + ⌊n*sqrt(3/5)⌋
 * Range:     u8 n <= 6, u16 n <= 390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_coeff.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for n < 1 */

    seq_t ns   = (seq_t)n;
    seq_t k1   = floor_sqrt_coeff(ns, (seq_t)3, (seq_t)5); /* ⌊n*sqrt(5/3)⌋ */
    seq_t k2   = floor_sqrt_coeff(ns, (seq_t)5, (seq_t)3); /* ⌊n*sqrt(3/5)⌋ */
    return ns + k1 + k2;
}
