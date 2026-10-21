/*
 * A086866  Third column of A059450.
 * https://oeis.org/A086866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 9*n - 2) * 2^(n-4) for n ≥ 2 For n = 0,1 the
 *            sequence is a(0)=0, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Reduce the arithmetic to seq_t to avoid temporary overflows. */
    seq_t nt = (seq_t)n;
    seq_t val = nt * nt + (seq_t)9 * nt - (seq_t)2;

    if (n >= 4)
        val <<= (n - 4);          /* multiply by 2^(n-4) */
    else
        val >>= (4 - n);          /* divide by 2^(4-n) */

    return val;
}
