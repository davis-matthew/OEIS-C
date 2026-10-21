/*
 * A205459  Number of (n+1) X 2 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock differing from each horizontal
 *          or vertical neighbor.
 * https://oeis.org/A205459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   block_count, ensure_results_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Number of (n+1)×2 arrays over {0,1,2,3} such that
 * the numbers of clockwise edge increases in consecutive 2×2 subblocks
 * are all different.  (OEIS A205459)
 *
 * This implementation uses a transfer‑matrix DP over the two rows
 * of a subblock.  States are a pair of rows (16 possibilities) and
 * the count of clockwise edge increases in the previous subblock
 * (0…4).  The DP is memoised so that subsequent calls to a() return
 * immediately even if called with decreasing indices.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/block_count.h"
#include "../utils/ensure_results_cap.h"

/* Main function asked for. */
seq_t a(idx_t n)
{
    /* a(0) is undefined – return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static state for incremental DP. */
    static idx_t dp_rows = 0;          /* number of rows processed in DP */
    static seq_t *dp_prev = NULL;      /* dp for current rows             */
    static seq_t *dp_cur  = NULL;      /* temporary dp                    */
    static idx_t max_n_computed = 0;   /* largest n for which a(n) stored */
    static seq_t *results = NULL;      /* results[1..max_n_computed]      */
    static size_t results_cap  = 0;    /* capacity of results array       */

    /* Allocate DP arrays on first use. */
    if (dp_prev == NULL) {
        dp_prev = (seq_t *)calloc(16 * 5, sizeof(seq_t));
        dp_cur  = (seq_t *)calloc(16 * 5, sizeof(seq_t));
        dp_rows = 0;
    }

    /* Extend the DP until we have computed a(n). */
    while (max_n_computed < n) {
        /* First time: initialise dp_prev for two rows (i=2). */
        if (dp_rows == 0) {
            /* Count all possible transitions from the first row to the
             * second row.  Each pair (A,B) contributes 1 to dp_prev[B][c]. */
            for (int A = 0; A < 16; ++A) {
                for (int B = 0; B < 16; ++B) {
                    int c = block_count(A, B);
                    dp_prev[B * 5 + c] += (seq_t)1;
                }
            }
            dp_rows = 2;   /* we now have two rows processed */
        } else {
            /* Transition from dp_prev (dp_rows rows) to dp_cur
             * (dp_rows+1 rows).  Reset dp_cur to zero. */
            memset(dp_cur, 0, 16 * 5 * sizeof(seq_t));

            for (int P = 0; P < 16; ++P) {
                for (int c_prev = 0; c_prev <= 4; ++c_prev) {
                    seq_t val = dp_prev[P * 5 + c_prev];
                    if (val == (seq_t)0)
                        continue;
                    for (int Q = 0; Q < 16; ++Q) {
                        int c_new = block_count(P, Q);
                        if (c_new != c_prev) {
                            dp_cur[Q * 5 + c_new] += val;
                        }
                    }
                }
            }
            /* Swap dp_cur into dp_prev for next iteration. */
            seq_t *tmp = dp_prev;
            dp_prev = dp_cur;
            dp_cur  = tmp;
            dp_rows++;
        }

        /* Compute a(dp_rows-1) as the sum over all states of dp_prev. */
        seq_t total = (seq_t)0;
        for (int P = 0; P < 16; ++P) {
            for (int c = 0; c <= 4; ++c) {
                total += dp_prev[P * 5 + c];
            }
        }

        /* Store the result. */
        ensure_results_cap(&results_cap, &results, dp_rows - 1);
        results[dp_rows - 1] = total;
        max_n_computed = dp_rows - 1;
    }

    /* Return the cached value. */
    return results[n];
}
