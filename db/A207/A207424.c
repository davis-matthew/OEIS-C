/*
 * A207424  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary matrices avoiding the forbidden
 *            horizontal triples 000 and 010, and the forbidden vertical
 *            triples 001 and 010. Computed by dynamic programming over pairs
 *            of rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----------  Initialization of the state space  ---------- */
    static bool initialized = false;
    static int rows[64];          /* indices of all valid 6‑bit rows   */
    static size_t rows_cnt;       /* number of valid rows             */
    static size_t M;              /* rows_cnt * rows_cnt              */
    static uint8_t *trans_next;   /* flattened [M][64] transition list */
    static uint8_t *trans_count;  /* transition #for each pair         */

    if (!initialized) {
        /* find all rows that satisfy the horizontal constraints   */
        rows_cnt = 0;
        for (int r = 0; r < 64; ++r) {
            bool good = true;
            for (int j = 0; j <= 6 - 3; ++j) {
                int t = (r >> j) & 7;          /* 3‑bit window          */
                if (t == 0 || t == 2) {        /* 000 or 010 impossible*/
                    good = false;
                    break;
                }
            }
            if (good) {
                rows[rows_cnt++] = r;
            }
        }

        M = rows_cnt * rows_cnt;
        trans_next  = (uint8_t *)malloc(M * 64 * sizeof(uint8_t));
        trans_count = (uint8_t *)malloc(M * sizeof(uint8_t));

        /* pre‑compute allowed transitions for each pair of rows   */
        for (size_t i = 0; i < rows_cnt; ++i) {
            for (size_t j = 0; j < rows_cnt; ++j) {
                size_t pid = i * rows_cnt + j;
                size_t cnt = 0;
                for (size_t l = 0; l < rows_cnt; ++l) {
                    bool ok = true;
                    for (int k = 0; k < 6; ++k) {
                        int a = (rows[i] >> k) & 1;
                        int b = (rows[j] >> k) & 1;
                        int c = (rows[l] >> k) & 1;
                        if ((a == 0 && b == 0 && c == 1) ||
                            (a == 0 && b == 1 && c == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        trans_next[pid * 64 + cnt++] = (uint8_t)l;
                    }
                }
                trans_count[pid] = (uint8_t)cnt;
            }
        }
        initialized = true;
    }

    /* ----------  Special cases for n = 1 and n = 2  ---------- */
    if (n == 1) {
        return (seq_t)rows_cnt;
    }
    if (n == 2) {
        seq_t a = (seq_t)rows_cnt;
        return a * a;         /* (rows_cnt)^2 */
    }

    /* ----------  Dynamic programming over rows  ---------- */
    size_t steps = (size_t)n - 2;          /* number of transitions */
    seq_t *dp      = (seq_t *)calloc(M, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(M, sizeof(seq_t));
    if (!dp || !dp_next) {                 /* memory allocation failed */
        return 0;
    }

    /* initialize for two rows : every pair is allowed once    */
    for (size_t p = 0; p < M; ++p) {
        dp[p] = 1;
    }

    for (size_t step = 0; step < steps; ++step) {
        /* zero next array */
        for (size_t p = 0; p < M; ++p) {
            dp_next[p] = 0;
        }

        for (size_t pid = 0; pid < M; ++pid) {
            seq_t cur = dp[pid];
            if (cur == 0) continue;
            size_t j = pid % rows_cnt;            /* second row index */
            uint8_t cnt = trans_count[pid];
            uint8_t *nb = &trans_next[pid * 64];
            for (uint8_t idx = 0; idx < cnt; ++idx) {
                size_t l = nb[idx];
                size_t np = j * rows_cnt + l;
                dp_next[np] += cur;
            }
        }
        /* swap dp and dp_next */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* sum all states to obtain total number of matrices */
    seq_t result = 0;
    for (size_t p = 0; p < M; ++p) {
        result += dp[p];
    }

    free(dp);
    free(dp_next);
    return result;
}
