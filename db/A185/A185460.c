/*
 * A185460  Number of (n+1) X 3 binary arrays with no 2 X 2 subblock
 *          determinant equal to any horizontal or vertical neighbor 2 X 2
 *          subblock determinant.
 * https://oeis.org/A185460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   det_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=34, a(2)=92, ...; computable via DP over 3‑bit rows. */

#include <stdint.h>
#include "../utils/det_pair.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1, but guard for safety */
        return 0;

    /* DP arrays: dp[prev_row][curr_row] */
    seq_t dp[8][8] = {0}, ndp[8][8] = {0};
    int dl_prev, dr_prev, dl_new, dr_new;

    /* base: n = 1, only one pair of rows */
    for (idx_t r0 = 0; r0 < 8; ++r0) {
        for (idx_t r1 = 0; r1 < 8; ++r1) {
            det_pair(r0, r1, &dl_prev, &dr_prev);
            if (dl_prev != dr_prev)
                dp[r0][r1] = 1;
        }
    }

    if (n == 1) {
        seq_t total = 0;
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = 0; j < 8; ++j)
                total += dp[i][j];
        return total;
    }

    /* Build for rows 2..n (total n+1 rows -> n pairs) */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear ndp */
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = 0; j < 8; ++j)
                ndp[i][j] = 0;

        for (idx_t prev = 0; prev < 8; ++prev) {
            for (idx_t curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;

                det_pair(prev, curr, &dl_prev, &dr_prev);

                for (idx_t nxt = 0; nxt < 8; ++nxt) {
                    det_pair(curr, nxt, &dl_new, &dr_new);
                    /* horizontal: left != right in the new pair */
                    if (dl_new == dr_new) continue;
                    /* vertical: new determinants differ from previous ones */
                    if (dl_new == dl_prev) continue;
                    if (dr_new == dr_prev) continue;
                    ndp[curr][nxt] += ways;
                }
            }
        }
        /* swap dp and ndp */
        for (idx_t i = 0; i < 8; ++i)
            for (idx_t j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all states after processing n pairs */
    seq_t total = 0;
    for (idx_t i = 0; i < 8; ++i)
        for (idx_t j = 0; j < 8; ++j)
            total += dp[i][j];

    return total;
}
