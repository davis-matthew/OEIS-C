/*
 * A202891  Number of (n+2)X3 binary arrays avoiding patterns 001 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A202891 – number of (n+2)×3 binary arrays avoiding the patterns
 *            001 and 010 in every row, in every column and in every NW‑to‑SE
 *            diagonal. Computed by a transfer‑matrix DP over the last two
 *            rows (64 possible states). The recurrence initial values are
 *            those given in the OEIS entry for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   STATE
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/STATE.h"

static const bool okRow[8] = {
    true,  /* 000 */
    false, /* 001 forbidden (left‑to‑right) */
    false, /* 010 forbidden */
    true,  /* 011 */
    true,  /* 100 */
    true,  /* 101 */
    true,  /* 110 */
    true   /* 111 */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of bound, safe */

    /* Number of rows in the array */
    idx_t N = n + 2;

    /* dp_cur[state] – number of ways to reach the current
       row having the last two rows encoded in 'state' */
    seq_t dp_cur[64] = {0};
    seq_t dp_next[64];

    /* Initialise with all pairs of valid rows a (row 1)
       and b (row 2) */
    for (int a = 0; a < 8; ++a)
        if (okRow[a])
            for (int b = 0; b < 8; ++b)
                if (okRow[b])
                    dp_cur[STATE(a, b)] = 1;

    /* Build rows 3 … N */
    for (idx_t row = 3; row <= N; ++row) {
        for (int i = 0; i < 64; ++i) dp_next[i] = 0;

        for (int a = 0; a < 8; ++a)
            if (okRow[a])
                for (int b = 0; b < 8; ++b)
                    if (okRow[b]) {
                        seq_t ways = dp_cur[STATE(a, b)];
                        if (ways == 0) continue;

                        for (int c = 0; c < 8; ++c)
                            if (okRow[c]) {
                                /* Vertical check – columns */
                                bool bad = false;
                                for (int j = 0; j < 3; ++j) {
                                    int aj = (a >> (2 - j)) & 1; /* column j */
                                    int bj = (b >> (2 - j)) & 1;
                                    int cj = (c >> (2 - j)) & 1;
                                    if (aj == 0 && bj == 0 && cj == 1) { bad = true; break; }
                                    if (aj == 0 && bj == 1 && cj == 0) { bad = true; break; }
                                }
                                if (bad) continue;

                                /* NW‑to‑SE diagonal check */
                                int a0 = (a >> 2) & 1;      /* column 0 (leftmost) */
                                int b1 = (b >> 1) & 1;      /* column 1 */
                                int c2 = c & 1;             /* column 2 (rightmost) */
                                if (a0 == 0 && b1 == 0 && c2 == 1) continue;
                                if (a0 == 0 && b1 == 1 && c2 == 0) continue;

                                dp_next[STATE(b, c)] += ways;
                            }
                    }

        /* advance to next row */
        for (int i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all final states – all rows built */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i) total += dp_cur[i];
    return total;
}
