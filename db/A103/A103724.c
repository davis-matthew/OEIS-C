/*
 * A103724  Column m=7 sequence of triangle A103718(n,m), n>=0, without
 *          leading zeros.
 * https://oeis.org/A103724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |S1(n+8,8)| + |S1(n+8,9)|
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes unsigned Stirling numbers of the first kind by the
   recurrence s(0,0)=1, s(n,0)=0 (n>0) and
   s(n,k)=s(n-1,k-1)+(n-1)*s(n-1,k).  Only columns 0…9 are needed. */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* We need s(n+8,8) and s(n+8,9).  Compute all rows up to n+8. */
    idx_t target = n + 8;

    /* Arrays for the current and next row: indices 0…9 (k). */
    seq_t cur[10] = {0};
    seq_t nxt[10] = {0};

    /* Base case: s(0,0) = 1 */
    cur[0] = (seq_t)1;

    for (idx_t i = 1; i <= target; ++i) {
        /* At row i we need k = 1 … min(i,9) */
        idx_t maxk = (i < 9) ? i : 9;

        /* Compute next row */
        nxt[0] = (seq_t)0;
        for (idx_t k = 1; k <= maxk; ++k) {
            /* s(i,k) = s(i-1,k-1) + (i-1)*s(i-1,k) */
            nxt[k] = cur[k-1] + (seq_t)(i - 1) * cur[k];
        }
        /* For k > maxk the values are zero.  Copy back to cur. */
        for (idx_t k = 0; k <= maxk; ++k) cur[k] = nxt[k];
        for (idx_t k = maxk + 1; k <= 9; ++k) cur[k] = (seq_t)0;
    }

    /* At this point cur[8] = s(target,8) and cur[9] = s(target,9) */
    return cur[8] + cur[9];
}
