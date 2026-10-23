/*
 * A180459  Sampling n numbers between 1 and a(n)-1, you are guaranteed to
 *          always find two subsets whose sums are equal.
 * https://oeis.org/A180459
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( (2^n - 2 + n*(n-1)/2) / n ) for n >= 3 (for n < 3
 *            the function returns 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 3)
        return (seq_t)0;

    seq_t pow2n  = pow2(n);          /* 2^n  */
    pow2n -= (seq_t)2;               /* 2^n - 2 */

    seq_t nseq = (seq_t)n;
    seq_t half = (seq_t)2;
    seq_t t = nseq * (nseq - (seq_t)1); /* n*(n-1) */
    t /= half;                         /* n*(n-1)/2 */

    seq_t numerator = pow2n + t;       /* 2^n - 2 + n*(n-1)/2 */
    seq_t result = (numerator + nseq - (seq_t)1) / nseq; /* ceil division */
    return result;
}
