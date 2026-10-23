/*
 * A189613  Number of nX5 binary arrays without the pattern 0 1 0 diagonally,
 *          antidiagonally or horizontally.
 * https://oeis.org/A189613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A189613: number of n×5 binary arrays without the
   pattern 0 1 0 horizontally, diagonally, or anti‑diagonally. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ----- static data, initialized once --------------------------------- */
    static bool initialized = false;
    static bool allowedRow[32];                /* true if row has no 010 */
    static uint8_t trans_cnt[32][32];          /* # of allowed third rows */
    static uint8_t trans_next[32][32][32];    /* list of allowed third rows */
    static uint8_t countAllowed = 0;          /* number of allowed rows */

    if (!initialized) {
        /* Determine which 5‑bit rows contain no horizontal 010. */
        for (int row = 0; row < 32; ++row) {
            bool ok = true;
            for (int j = 0; j <= 2; ++j) {
                int sub = (row >> j) & 7;          /* 3‑bit window */
                if (sub == 0b010) { ok = false; break; }
            }
            allowedRow[row] = ok;
            if (ok) ++countAllowed;
        }

        /* Precompute possible transitions (a,b) → c. */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                if (!allowedRow[a] || !allowedRow[b]) {
                    trans_cnt[a][b] = 0;
                    continue;
                }
                uint8_t k = 0;
                for (int c = 0; c < 32; ++c) {
                    if (!allowedRow[c]) continue;
                    bool ok = true;
                    for (int j = 0; j <= 2; ++j) {
                        int a_bit  = (a >> j)   & 1;
                        int b_bit  = (b >> (j+1)) & 1;
                        int c_bit  = (c >> (j+2)) & 1;
                        if (a_bit == 0 && b_bit == 1 && c_bit == 0) { ok = false; break; }
                        int a_anti = (a >> (j+2)) & 1;
                        int b_anti = (b >> (j+1)) & 1;
                        int c_anti = (c >> j)     & 1;
                        if (a_anti == 0 && b_anti == 1 && c_anti == 0) { ok = false; break; }
                    }
                    if (ok) {
                        trans_next[a][b][k++] = (uint8_t)c;
                    }
                }
                trans_cnt[a][b] = k;
            }
        }
        initialized = true;
    }
    /* --------------------------------------------------------------------- */

    if (n == 0) return 0;          /* below offset – not used in tests */
    if (n == 1) return (seq_t)countAllowed;
    if (n == 2) return (seq_t)countAllowed * countAllowed;

    /* DP arrays over pairs of rows. 32 × 32 is very small. */
    static seq_t dp_cur[32][32];
    static seq_t dp_next[32][32];

    /* Initialise dp_cur for the first two rows. */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            dp_cur[a][b] = 0;
    for (int a = 0; a < 32; ++a) if (allowedRow[a])
        for (int b = 0; b < 32; ++b) if (allowedRow[b])
            dp_cur[a][b] = 1;

    /* Process rows 3 … n. */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero next layer. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_next[a][b] = 0;

        /* Transition. */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t val = dp_cur[a][b];
                if (val == 0) continue;
                uint8_t k = trans_cnt[a][b];
                for (uint8_t t = 0; t < k; ++t) {
                    int c = trans_next[a][b][t];
                    dp_next[b][c] += val;
                }
            }
        }

        /* Move dp_next → dp_cur for next iteration. */
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                dp_cur[a][b] = dp_next[a][b];
    }

    /* Sum all counts for the final row. */
    seq_t ans = 0;
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            ans += dp_cur[a][b];

    return ans;
}
