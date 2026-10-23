/*
 * A009440  a(n) is the concatenation of n and 6n.
 * https://oeis.org/A009440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = concat(n, 6*n)
 * Range:     u8 n <= 2, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;          /* convert index to seq_t */
    seq_t m = nseq * (seq_t)6;      /* 6 * n */
    int d = digit_len(m);           /* number of decimal digits of 6n */
    seq_t p10 = pow10((idx_t)d);    /* 10^d */
    return nseq * p10 + m;          /* concatenate: n * 10^d + 6n */
}
