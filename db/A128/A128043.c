/*
 * A128043  a(n) = (n^3 + n)*6^n.
 * https://oeis.org/A128043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n)*6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 43,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n==0 the value is irrelevant, we return 0. */
    if (n == 0)
        return 0U;

    seq_t nseq = (seq_t)n;                     /* cast for exact arithmetic   */
    seq_t part = nseq * nseq * nseq + nseq;    /* n^3 + n                       */
    seq_t pow6  = seq_pow((seq_t)6, n);        /* 6^n                            */
    return part * pow6;                        /* (n^3 + n)*6^n                 */
}
