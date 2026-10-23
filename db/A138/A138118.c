/*
 * A138118  Concatenation of 2n-1 digits 1 and n digits 0.
 * https://oeis.org/A138118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10*a(n-1) + 11*1000^(n-1) (a(1) = 10)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)10;          /* a(1) */
    if (n == 1)
        return val;

    seq_t pow1000 = (seq_t)1;       /* 1000^(i-1) for the current loop index i */
    for (idx_t i = 2; i <= n; ++i) {
        pow1000 *= (seq_t)1000;                 /* 1000^(i-1) */
        val = val * (seq_t)10 + (seq_t)11 * pow1000;
    }
    return val;
}
