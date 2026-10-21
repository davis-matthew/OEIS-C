/*
 * A077626  Largest term in periodic part of continued fraction expansion of
 *          square root of 1+3^n or 0 if 1+3^n is square.
 * https://oeis.org/A077626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if 1+3^n is a perfect square, otherwise 2 *
 *            floor(sqrt(1+3^n))
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   floor_sqrt, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 3^n */
    seq_t power = seq_pow((seq_t)3, n);

    /* D = 1 + 3^n */
    seq_t D = power + (seq_t)1;

    /* floor(sqrt(D)) */
    seq_t a0 = floor_sqrt(D);

    /* Check if D is a perfect square */
    if (a0 * a0 == D)
        return (seq_t)0;

    /* Largest term in periodic part of continued fraction of sqrt(D) */
    return a0 * (seq_t)2;
}
