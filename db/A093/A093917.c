/*
 * A093917  a(n) = n^3+n for odd n, (n^3+n)*3/2 for even n: Row sums of
 *          A093915.
 * https://oeis.org/A093917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + n for odd n; a(n) = (n^3 + n) * 3 / 2 for even n
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently: a(2n-1) = 2*(2n-1)*(2n^2-2n+1),  a(2n) = 3*n*(4n^2+1) ) */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;            /* convert index to seq_t type      */
    seq_t cube  = nseq * nseq * nseq; /* n^3                               */
    seq_t val   = cube + nseq;        /* n^3 + n                           */

    if ((n & 1) == 0) {               /* even n: multiply by 3, divide by 2 */
        return (val * 3) / 2;
    } else {
        return val;                    /* odd n: just n^3 + n               */
    }
}
