/*
 * A185456  Payphone packing sequence.
 * https://oeis.org/A185456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 2^(1+floor(log_2(n-2))) for n>=3, with a(1)=1 and
 *            a(2)=3
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* m = n-2 >= 1 for n>=3 */
    idx_t m = n - 2;

    /* floor(log2(m)) using BSR (builtin clzll) */
    unsigned int l = 63 - __builtin_clzll(m);   /* l = floor(log2(m)) */

    /* 2^(1+l) = 1 << (l+1) */
    seq_t pow2 = ((seq_t)1) << (l + 1);

    return (seq_t)n + pow2;
}
