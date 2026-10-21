/*
 * A087159  Satisfies a(1)=1, a(A087160(n+1)) = a(n)+1, with a(m)=2 for all m
 *          not found in A087160, where A087160(n+2)=A087160(n+1)+a(n)+1.
 * https://oeis.org/A087159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(m)=2 for all m not in A087160, and a(A087160(n+1)) =
 *            a(n)+1. The sequence of indices A087160 satisfies B(1)=1,
 *            B(2)=2, B(n+2)=B(n+1)+a(n)+1. The code computes a(n) by
 *            regenerating the required values up to n on each call
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of bounds guard */

    /* allocate array up to n (index 1..n) */
    seq_t *seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq) return 0;           /* allocation failure guard */

    /* initialise all to 2; then set a(1)=1 */
    for (idx_t i = 1; i <= n; ++i) seq[i] = (seq_t)2;
    seq[1] = (seq_t)1;            /* a(1) = 1 */

    /* B(1)=1, B(2)=2; start processing from the second increment */
    idx_t incCnt = 2;              /* number of increments processed so far */
    idx_t B_last = 2;              /* current last increment index */

    /* generate further increments while they fit within the requested range */
    while (B_last <= n) {
        idx_t nextIdx = B_last + (idx_t)seq[incCnt - 1] + 1;
        if (nextIdx > n) break;          /* no more increments needed */

        seq[nextIdx] = seq[incCnt] + (seq_t)1; /* a(B_{k+1}) = a(k-1)+1 */
        incCnt++;
        B_last = nextIdx;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
