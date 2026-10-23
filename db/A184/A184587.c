/*
 * A184587  a(n) = floor((n+1/2)*s), where s=(5+sqrt(5))/4; complement of
 *          A184586.
 * https://oeis.org/A184587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1/2)*s) where s = (5+sqrt(5))/4
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Computes the nth element of OEIS sequence A184587. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 we return 0. */
    if (n == 0) return 0;

    /* Work in seq_t to avoid overflow on intermediate steps. */
    seq_t seqn = (seq_t)n;          /* seqn = n  */
    seq_t m     = seqn * 2 + 1;      /* m = 2n+1  */
    seq_t five_m = m * 5;           /* five_m = 5*(2n+1)  */
    seq_t t      = five_m * m;      /* t = 5*(2n+1)^2  */

    /* floor((2n+1)*sqrt(5)) = floor(sqrt(5*(2n+1)^2)) */
    seq_t sqrt_term = floor_sqrt(t);

    /* Numerator of the formula: 5*(2n+1) + floor((2n+1)*sqrt(5)) */
    seq_t num = five_m + sqrt_term;

    /* Final result: floor(num / 8) */
    return num / 8;
}
