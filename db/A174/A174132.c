/*
 * A174132  a(n) = 2*3^(n-1) - (-1)^n.
 * https://oeis.org/A174132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^(n-1) - (-1)^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 3^(n-1) using the provided big_pow helper */
    seq_t pow_val = big_pow((seq_t)3, (seq_t)(n - 1));

    /* 2 * 3^(n-1) */
    seq_t result = (seq_t)2 * pow_val;

    /* Adjust by (-1)^n: add 1 if n is odd, subtract 1 if n is even */
    if (n & 1U)        /* odd n */
        result += (seq_t)1;
    else               /* even n */
        result -= (seq_t)1;

    return result;
}
