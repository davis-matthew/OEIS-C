/*
 * A060195  a(n) = 8^(n-1)*(2^n - 1).
 * https://oeis.org/A060195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n-1) * (2^n - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of A060195.
 * The sequence is defined for n ≥ 1; for n = 0 we return 0. */
seq_t a(idx_t n)
{
    /* Handle the forbidden offset */
    if (n == 0)
        return (seq_t)0;

    /* 8^(n-1) */
    seq_t pow8 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow8 *= (seq_t)8;

    /* 2^n */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2 *= (seq_t)2;

    /* (2^n - 1) */
    seq_t sub = pow2 - (seq_t)1;

    /* Final product: 8^(n-1) * (2^n - 1) */
    return pow8 * sub;
}
