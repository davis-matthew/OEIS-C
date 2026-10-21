/*
 * A236823  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..2 such that *
 *            for every 2×2 subblock the value max+min−lower‑median is the
 *            same * (0, 1 or 2). The count is obtained by dynamic programming
 *            over * 81 possible row states (4 digits base‑3). For each
 *            f∈{0,1,2} we * build a transition graph of allowed...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1 */

    /* -----  State encodings (4 digits in base 3) ----- */
    static uint8_t row_digits[81][4];
    static bool init = false;

    /* -----  Transition tables for f = 0, 1, 2  ----- */
    static uint8_t  trans_cnt[3][81];
    static idx_t    trans[3][81][81];      /* dest indices */

    if (!init) {
        /* pre‑compute the 4‑digit patterns for each state */
        for (idx_t r = 0; r < 81; ++r) {
            idx_t x = r;
            for (int p = 3; p >= 0; --p) {
                row_digits[r][p] = (uint8_t)(x % 3);
                x /= 3;
            }
        }

        /* pre‑compute allowed transitions for f = 0,1,2 */
        for (int f = 0; f < 3; ++f) {
            for (idx_t i = 0; i < 81; ++i) {
                const uint8_t *a = row_digits[i];
                for (idx_t j = 0; j < 81; ++j) {
                    const uint8_t *b = row_digits[j];
                    bool ok = true;
                    for (int c = 0; c < 3; ++c) {
                        int v1 = a[c],   v2 = a[c+1];
                        int v3 = b[c],   v4 = b[c+1];

                        /* counts of 0,1,2 in the 4 elements */
                        int c0 = 0, c1 = 0, c2 = 0;
                        if (v1 == 0) ++c0; else if (v1 == 1) ++c1; else ++c2;
                        if (v2 == 0) ++c0; else if (v2 == 1) ++c1; else ++c2;
                        if (v3 == 0) ++c0; else if (v3 == 1) ++c1; else ++c2;
                        if (v4 == 0) ++c0; else if (v4 == 1) ++c1; else ++c2;

                        /* min, max and lower median of the 4 numbers */
                        int minv  = c0 > 0 ? 0 : (c1 > 0 ? 1 : 2);
                        int maxv  = c2 > 0 ? 2 : (c1 > 0 ? 1 : 0);
                        int lower;
                        if (c0 >= 2)       lower = 0;
                        else if (c0 == 1)  lower = (c1 > 0 ? 1 : 2);
                        else if (c1 >= 2)  lower = 1;
                        else               lower = 2;

                        int fv = maxv + minv - lower;
                        if (fv != f) { ok = false; break; }
                    }
                    if (ok) {
                        trans_cnt[f][i] = trans_cnt[f][i] + 1;
                        trans[f][i][trans_cnt[f][i]-1] = j;
                    }
                }
            }
        }
        init = true;
    }

    seq_t answer = 0;

    /* ----- dynamic programming for each f ----- */
    for (int f = 0; f < 3; ++f) {
        seq_t dp[81];
        seq_t next[81];

        /* first row can be anything */
        for (idx_t i = 0; i < 81; ++i) dp[i] = 1;

        for (idx_t step = 0; step < n; ++step) {
            for (idx_t i = 0; i < 81; ++i) next[i] = 0;

            for (idx_t i = 0; i < 81; ++i) {
                seq_t val = dp[i];
                if (val == 0) continue;
                uint8_t tcnt = trans_cnt[f][i];
                for (uint8_t t = 0; t < tcnt; ++t) {
                    idx_t j = trans[f][i][t];
                    next[j] = next[j] + val;
                }
            }

            for (idx_t i = 0; i < 81; ++i) dp[i] = next[i];
        }

        /* sum of all ways for this f */
        seq_t sum_f = 0;
        for (idx_t i = 0; i < 81; ++i) sum_f = sum_f + dp[i];
        answer = answer + sum_f;
    }

    return answer;
}
