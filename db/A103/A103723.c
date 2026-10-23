/*
 * A103723  Column m=6 sequence of triangle A103718(n,m), n>=0.
 * https://oeis.org/A103723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A103723: Column m = 6 of A103718.
    a(n) = |S1(n+7,7)| + |S1(n+7,8)|  (Stirling numbers of the first kind, absolute value)
    Using the standard recurrence:
        S1(0,0) = 1,  S1(n,0) = 0 (n>0)
        S1(n,k) = S1(n-1,k-1) + (n-1)*S1(n-1,k)
*/

#include <stdint.h>

seq_t a(idx_t n) {
    /* The maximal row we need for the recurrence is n+7,
       because we need values S1(n+7,7) and S1(n+7,8).   */
    idx_t target = n + 7;          /* last row index to compute */
    /* A small fixed‑size table for k = 0..8.  */
    seq_t prev[9] = {0};
    prev[0] = 1;                    /* S1(0,0) = 1 */

    /* Iterate rows 1..target.  Each step builds the next row from prev.
       Only k up to 8 are required.                                         */
    for (idx_t r = 1; r <= target; ++r) {
        seq_t next[9] = {0};
        idx_t maxK = r < 8 ? r : 8;
        for (idx_t k = 1; k <= maxK; ++k) {
            /* next[k] = prev[k-1] + (r-1) * prev[k] */
            seq_t mult = prev[k] * (seq_t)(r - 1);
            next[k] = prev[k - 1] + mult;
        }
        /* Copy next to prev for the next iteration. */
        for (idx_t k = 0; k < 9; ++k) {
            prev[k] = next[k];
        }
    }

    /* At this point prev holds S1(target, k) for k up to 8.
       For a(n) we need S1(n+7,7) + S1(n+7,8) = prev[7] + prev[8].        */
    return prev[7] + prev[8];
}
