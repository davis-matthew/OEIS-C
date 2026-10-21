/*
 * A207429  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207429
 *
 * 5×n binary arrays avoiding
 *   horizontal patterns 0 0 0 and 0 1 0,
 *   vertical   patterns 0 0 1 and 0 1 0.
 *
 * The implementation builds all column patterns that satisfy the vertical
 * constraints (there are at most 32).  Then it uses a width‑dependent DP
 * over pairs of successive columns.  For each transition (a,b,c) it tests
 * the horizontal constraints on the three columns.  This yields the
 * correct count for any n≥1 without memoising between calls, but with
 * only O(K²) memory (K≤32) and O(n·K³) time, easily below 2 seconds for
 * n up to 10 000.
 *
 * The function's signature is required by the test harness:
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------
     *  Helper structures and static initialisation
     * -------------------------------------------------------------- */
    static bool initialized = false;
    static idx_t K;                     /* number of vertical‑valid columns */
    static unsigned int *valid_cols;    /* array of K column bit patterns */
    static idx_t pair_cnt;              /* K*K */
    static seq_t *dp_cur = NULL;        /* DP for current length   */
    static seq_t *dp_next = NULL;       /* DP for next length     */

    /* --------------------------------------------------------------- */
    if (!initialized) {
        /* --- Enumerate all 5‑bit columns that satisfy the vertical
         *     forbidden patterns 0 0 1 and 0 1 0. ------------------- */
        unsigned int temp[32];
        size_t vcnt = 0;
        for (unsigned int col = 0; col < 32; ++col) {
            bool ok = true;
            for (int r = 0; r <= 2 && ok; ++r) {
                int b0 = (col >> r)     & 1;
                int b1 = (col >> (r+1)) & 1;
                int b2 = (col >> (r+2)) & 1;
                if ((b0==0 && b1==0 && b2==1) ||
                    (b0==0 && b1==1 && b2==0))
                    ok = false;
            }
            if (ok) temp[vcnt++] = col;
        }
        K = (idx_t)vcnt;
        valid_cols = (unsigned int*)malloc(K * sizeof(unsigned int));
        for (size_t i = 0; i < K; ++i) valid_cols[i] = temp[i];

        pair_cnt = K * K;
        dp_cur  = (seq_t*)calloc(pair_cnt, sizeof(seq_t));
        dp_next = (seq_t*)calloc(pair_cnt, sizeof(seq_t));
        initialized = true;
    }

    /* --------------------------------------------------------------- */
    /*  Special cases: n == 1 or n == 2 (horizontal constraints require
     *  at least 3 columns). --------------------------------------- */
    if (n == 0) return (seq_t)0; /* outside offset, undefined but safe */
    if (n == 1) return (seq_t)K;
    if (n == 2) return (seq_t)K * (seq_t)K;

    /* --------------------------------------------------------------- */
    /*  Initialise dp for length = 2 : every pair of vertical‑valid
     *  columns starts with exactly one arrangement. ----------------- */
    for (idx_t i = 0; i < pair_cnt; ++i) dp_cur[i] = (seq_t)1;

    /* --------------------------------------------------------------- */
    /*  Extend the array step by step until length n. ---------------- */
    for (idx_t len = 3; len <= n; ++len) {
        for (idx_t i = 0; i < pair_cnt; ++i) dp_next[i] = (seq_t)0;

        for (idx_t a = 0; a < K; ++a) {
            for (idx_t b = 0; b < K; ++b) {
                seq_t ways = dp_cur[a * K + b];
                if (!ways) continue;

                unsigned int col_a = valid_cols[a];
                unsigned int col_b = valid_cols[b];

                for (idx_t c = 0; c < K; ++c) {
                    unsigned int col_c = valid_cols[c];
                    bool ok = true;

                    /* Check horizontal constraints for all 5 rows. */
                    for (int r = 0; r < 5; ++r) {
                        int da = (col_a >> r) & 1;
                        int db = (col_b >> r) & 1;
                        int dc = (col_c >> r) & 1;
                        if ((da==0 && db==0 && dc==0) ||
                            (da==0 && db==1 && dc==0)) {
                            ok = false;
                            break;
                        }
                    }

                    if (ok) {
                        idx_t dest = b * K + c;
                        dp_next[dest] = dp_next[dest] + ways;
                    }
                }
            }
        }
        /*  Prepare for next iteration. ------------------------------ */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* --------------------------------------------------------------- */
    /*  Sum all end states to obtain the total number of arrays. ----- */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < pair_cnt; ++i) total = total + dp_cur[i];

    return total;
}
