/*
 * A194986  a(n) = 1 + floor(n/sqrt(6)).
 * https://oeis.org/A194986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n/sqrt(6))
 * Range:     u8 n <= 15, u16 n <= 426, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n == 0 is outside the OEIS offset; return 0. */
    if (n == 0) return 0;
    seq_t n2 = (seq_t)n * n;   /* n² */
    seq_t t  = n2 / 6;         /* floor(n² / 6) */
    seq_t m  = floor_sqrt(t);  /* floor(sqrt(t)) = floor(n / sqrt(6)) */
    return m + 1;
}
