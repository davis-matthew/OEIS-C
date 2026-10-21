/*
 * A208162  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×6 binary arrays with
 *   no horizontal 000 or 101
 *   no vertical 001 or 010
 * This is implemented by dynamic programming over the last two rows.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system */

seq_t a(idx_t n)
{
    /* static state used to initialise the transition data once */
    static bool init = false;
    static int pat_cnt;
    static int allowed[64];          /* masks of allowed rows */
    static unsigned int next_mask[64][64]; /* bits of allowed next rows */
    static seq_t dp[64][64];
    static seq_t ndp[64][64];

    if (!init) {
        /* enumerate all 6‑bit patterns that avoid 000 and 101 horizontally */
        pat_cnt = 0;
        for (int m = 0; m < 64; ++m) {
            bool ok = true;
            for (int pos = 0; pos < 4; ++pos) {
                int sub = (m >> pos) & 7;       /* three consecutive bits */
                if (sub == 0 || sub == 5) {      /* 000 or 101 */
                    ok = false;
                    break;
                }
            }
            if (ok)
                allowed[pat_cnt++] = m;
        }

        /* pre‑compute allowed transitions between pairs of rows */
        for (int i = 0; i < pat_cnt; ++i) {
            for (int j = 0; j < pat_cnt; ++j) {
                unsigned int mask = 0;
                int a = allowed[i], b = allowed[j];
                for (int k = 0; k < pat_cnt; ++k) {
                    int c = allowed[k];
                    bool vert_ok = true;
                    for (int pos = 0; pos < 6; ++pos) {
                        int a_bit = (a >> pos) & 1;
                        int b_bit = (b >> pos) & 1;
                        int c_bit = (c >> pos) & 1;
                        if (a_bit == 0 && b_bit == 0 && c_bit == 1)
                            { vert_ok = false; break; }
                        if (a_bit == 0 && b_bit == 1 && c_bit == 0)
                            { vert_ok = false; break; }
                    }
                    if (vert_ok)
                        mask |= (1U << k);
                }
                next_mask[i][j] = mask;
            }
        }
        init = true;
    }

    /* Special cases for n = 1 and n = 2, where vertical constraints do not apply */
    if (n == 1)
        return (seq_t)pat_cnt;
    if (n == 2) {
        seq_t val = (seq_t)pat_cnt;
        return val * val;
    }

    /* DP initialization for the first two rows */
    for (int i = 0; i < pat_cnt; ++i)
        for (int j = 0; j < pat_cnt; ++j)
            dp[i][j] = 1;

    /* DP over additional rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (int i = 0; i < pat_cnt; ++i) {
            for (int j = 0; j < pat_cnt; ++j) {
                seq_t cur = dp[i][j];
                if (cur == 0) continue;
                unsigned int mask = next_mask[i][j];
                if (!mask) continue;
                for (int k = 0; k < pat_cnt; ++k) {
                    if (mask & (1U << k))
                        ndp[j][k] += cur;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Sum over all final states */
    seq_t total = 0;
    for (int i = 0; i < pat_cnt; ++i)
        for (int j = 0; j < pat_cnt; ++j)
            total += dp[i][j];

    return total;
}
