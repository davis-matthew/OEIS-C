/*
 * A236291  Number of length n binary words that contain an even number of 0's
 *          or exactly two 1's.
 * https://oeis.org/A236291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 for n > 0: if n even: a(n) = 2^(n+1) / 4 if n odd: a(n)
 *            = (2^(n+1) - 2*n + 2*n^2) / 4 (Division is exact for all n ≥ 0)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* compute 2^(n+1) */
    seq_t pow2 = pow_uint( (uint64_t)2, n + 1 );

    if ( (n & 1ULL) == 0 )                /* n even */
    {
        return pow2 >> 2;                 /* divide by 4 */
    }
    else                                  /* n odd */
    {
        seq_t nseq  = (seq_t)n;
        seq_t two_n = nseq << 1;          /* 2*n */
        seq_t two_nsqr = (nseq * nseq) << 1; /* 2*n^2 */
        seq_t numerator = pow2 - two_n + two_nsqr;
        return numerator >> 2;            /* divide by 4 */
    }
}
