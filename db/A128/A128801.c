/*
 * A128801  a(n) = n*(n-1)*7^n.
 * https://oeis.org/A128801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 1 using the helper big_pow() */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 explicitly to avoid overflow on
       (n-1) when n is 0 (outside the offset). */
    if (n <= 1)
        return (seq_t)0;

    /* Compute 7^n (seq_t exponent must be cast from idx_t) */
    seq_t power = big_pow((seq_t)7, (seq_t)n);

    /* Compute n*(n-1) */
    seq_t mul = (seq_t)n * ((seq_t)n - 1);

    /* Final product */
    return mul * power;
}
