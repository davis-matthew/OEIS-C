/*
 * A168432  a(n) = n^5*(n^7 + 1)/2.
 * https://oeis.org/A168432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5*(n^7 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1722,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast index to seq_t for all arithmetic */
    seq_t nseq = (seq_t)n;

    /* Compute powers of n step by step */
    seq_t n2 = nseq * nseq;
    seq_t n3 = n2 * nseq;
    seq_t n4 = n3 * nseq;
    seq_t n5 = n4 * nseq;
    seq_t n7 = n5 * n2;   /* n^5 * n^2 = n^7 */

    /* The numerator n^5 * (n^7 + 1) is always even.
       Divide the even factor by 2 before doing the large product
       to avoid intermediate overflow. */
    if ((n & 1ULL) == 0) {
        /* n is even → n^5 is even */
        seq_t n5_half = n5 / (seq_t)2;
        /* a(n) = n5_half * (n7 + 1) */
        return n5_half * n7 + n5_half;
    } else {
        /* n is odd → n^7 + 1 is even */
        seq_t n7_half = (n7 + (seq_t)1) / (seq_t)2;
        /* a(n) = n5 * n7_half */
        return n5 * n7_half;
    }
}
