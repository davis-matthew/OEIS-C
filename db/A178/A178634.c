/*
 * A178634  a(n) = 63*((10^n - 1)/9)^2.
 * https://oeis.org/A178634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*((10^n - 1)/9)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper to compute 10^e using only seq_t operations. */
static seq_t pow10(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)0; i < e; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n)
{
    seq_t ten_pow_n   = pow10(n);
    seq_t repunit     = (ten_pow_n - (seq_t)1) / (seq_t)9;
    seq_t sq          = repunit * repunit;
    return sq * (seq_t)63;
}
