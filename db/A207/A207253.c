/*
 * A207253  Number of nX7 0..1 arrays avoiding 0 0 1 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over the valid 7‑bit
 *            rows. Rows may not contain 001 or 101 horizontally. Between
 *            consecutive rows the vertical patterns 010 and 101 are
 *            forbidden. The recurrence is a(1) = 14 a(2) = 14² a(n) =
 *            Σ_{row1,row2,row3} a(n-1, row1,row2) if row1,row2,row3 are
 *            compa...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_ROWS 14          /* number of admissible 7‑bit rows (14 for 7 columns) */

seq_t a(idx_t n)
{
    /* ---------- build admissible rows & transition table (once) ---------- */
    static bool init = false;
    static size_t R = 0;                     /* actual number of rows (≤ MAX_ROWS) */
    static idx_t rows[MAX_ROWS];
    static bool trans[MAX_ROWS][MAX_ROWS][MAX_ROWS];

    if (!init) {
        R = 0;
        for (idx_t val = 0; val < ((idx_t)1 << 7); ++val) {
            bool good = true;
            for (idx_t i = 0; i <= 4 && good; ++i) {
                idx_t bits = (val >> i) & 7;          /* 3‑bit window */
                if (bits == 0b001 || bits == 0b101)
                    good = false;
            }
            if (good) {
                rows[R++] = val;
            }
        }

        for (size_t a = 0; a < R; ++a)
            for (size_t b = 0; b < R; ++b)
                for (size_t c = 0; c < R; ++c) {
                    bool ok = true;
                    for (idx_t j = 0; j < 7; ++j) {
                        idx_t ak = (rows[a] >> j) & 1;
                        idx_t bk = (rows[b] >> j) & 1;
                        idx_t ck = (rows[c] >> j) & 1;
                        if (ak == ck && ak != bk) {      /* forbidden vertical pattern */
                            ok = false;
                            break;
                        }
                    }
                    trans[a][b][c] = ok;
                }
        init = true;
    }

    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)R;
    if (n == 2)
        return (seq_t)R * (seq_t)R;

    /* DP tables: dp_cur[prev][curr] = number of ways ending with rows (prev,curr) */
    static seq_t dp_cur[MAX_ROWS][MAX_ROWS];
    static seq_t dp_next[MAX_ROWS][MAX_ROWS];

    /* base: any pair of rows can be the first two rows */
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < R; ++j)
            dp_cur[i][j] = (seq_t)1;

    /* iterate rows 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear next DP */
        for (size_t i = 0; i < R; ++i)
            for (size_t j = 0; j < R; ++j)
                dp_next[i][j] = 0;

        /* transition */
        for (size_t prev = 0; prev < R; ++prev)
            for (size_t curr = 0; curr < R; ++curr) {
                seq_t cnt = dp_cur[prev][curr];
                if (cnt == 0)
                    continue;
                for (size_t nxt = 0; nxt < R; ++nxt)
                    if (trans[prev][curr][nxt])
                        dp_next[curr][nxt] += cnt;
            }

        /* copy next to cur */
        for (size_t i = 0; i < R; ++i)
            for (size_t j = 0; j < R; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* sum all final states */
    seq_t result = 0;
    for (size_t i = 0; i < R; ++i)
        for (size_t j = 0; j < R; ++j)
            result += dp_cur[i][j];

    return result;
}
