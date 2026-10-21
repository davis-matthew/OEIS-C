/*
 * A208837  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary matrices avoiding the horizontal
 *            patterns 000 and 001 and the vertical patterns 011 and 110.
 *            Computed via dynamic programming over rows
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special cases: offset is 1, so n<1 is undefined; return 0. */
    if (n < 1) return 0;

    /* All 5‑bit patterns. */
    #define PAT_COUNT 32
    static bool allowed[PAT_COUNT];
    static uint8_t allowed_list[PAT_COUNT];
    static int allowed_count = 0;

    /* Transition lists: for each pair (a,b) store allowed next rows c. */
    static uint8_t next_list[PAT_COUNT][PAT_COUNT][PAT_COUNT];
    static uint8_t next_count[PAT_COUNT][PAT_COUNT];
    static bool init = false;

    if (!init) {
        /* Compute allowed rows (horizontal constraint). */
        for (int p = 0; p < PAT_COUNT; ++p) {
            bool ok = true;
            for (int i = 0; i < 3; ++i) {            /* check positions 1‑4 */
                if (!(p >> i & 1) && !(p >> (i + 1) & 1)) {
                    ok = false;
                    break;
                }
            }
            allowed[p] = ok;
            if (ok) {
                allowed_list[allowed_count++] = (uint8_t)p;
            }
        }

        /* Precompute transitions respecting vertical constraint. */
        for (int a = 0; a < PAT_COUNT; ++a) {
            if (!allowed[a]) continue;
            for (int b = 0; b < PAT_COUNT; ++b) {
                if (!allowed[b]) continue;
                uint8_t *list = next_list[a][b];
                int cnt = 0;
                for (int c = 0; c < PAT_COUNT; ++c) {
                    if (!allowed[c]) continue;
                    bool banned = false;
                    for (int k = 0; k < 5; ++k) {
                        uint8_t ak = (a >> k) & 1U;
                        uint8_t bk = (b >> k) & 1U;
                        uint8_t ck = (c >> k) & 1U;
                        if (ak == 0 && bk == 1 && ck == 1) { banned = true; break; }
                        if (ak == 1 && bk == 1 && ck == 0) { banned = true; break; }
                    }
                    if (!banned) {
                        list[cnt++] = (uint8_t)c;
                    }
                }
                next_count[a][b] = (uint8_t)cnt;
            }
        }
        init = true;
    }

    /* DP tables: dp[a][b] = number of ways for last two rows being a, b. */
    static seq_t dp[PAT_COUNT][PAT_COUNT];
    static seq_t ndp[PAT_COUNT][PAT_COUNT];

    if (n == 1) {
        seq_t sum = 0;
        for (int idx = 0; idx < allowed_count; ++idx) {
            sum += 1;
        }
        return sum;
    }

    /* Initialize for n = 2 */
    for (int i = 0; i < PAT_COUNT; ++i)
        for (int j = 0; j < PAT_COUNT; ++j)
            dp[i][j] = 0;

    for (int i = 0; i < allowed_count; ++i) {
        int a = allowed_list[i];
        for (int j = 0; j < allowed_count; ++j) {
            int b = allowed_list[j];
            dp[a][b] = 1;
        }
    }

    if (n == 2) {
        seq_t sum = 0;
        for (int i = 0; i < PAT_COUNT; ++i)
            for (int j = 0; j < PAT_COUNT; ++j)
                sum += dp[i][j];
        return sum;
    }

    /* DP for rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero ndp */
        for (int i = 0; i < PAT_COUNT; ++i)
            for (int j = 0; j < PAT_COUNT; ++j)
                ndp[i][j] = 0;

        for (int a = 0; a < PAT_COUNT; ++a) {
            if (!allowed[a]) continue;
            for (int b = 0; b < PAT_COUNT; ++b) {
                if (!allowed[b]) continue;
                seq_t cur_val = dp[a][b];
                if (cur_val == 0) continue;
                uint8_t *list = next_list[a][b];
                int cnt = next_count[a][b];
                for (int t = 0; t < cnt; ++t) {
                    int c = list[t];
                    ndp[b][c] += cur_val;
                }
            }
        }
        /* Swap dp and ndp */
        for (int i = 0; i < PAT_COUNT; ++i)
            for (int j = 0; j < PAT_COUNT; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over all ending states */
    seq_t result = 0;
    for (int i = 0; i < PAT_COUNT; ++i)
        for (int j = 0; j < PAT_COUNT; ++j)
            result += dp[i][j];

    return result;
}
