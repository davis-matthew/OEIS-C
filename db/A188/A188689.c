/*
 * A188689  Number of nX3 binary arrays without the pattern 0 1 0 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7, a(2) = 49. For n ≥ 3, a(n) is obtained by a linear DP
 *            with state consisting of the last two rows of the 3‑column
 *            array. Each row is a 3‑bit pattern (0…7) that must not be 010
 *            horizontally. When a new row is added we check: – vertical 0 1 0
 *            in each column (rows k–2, k–1, k) – diagonal do...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   rows_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------
   The following function implements the above algorithm.
   It works for any seq_t and idx_t defined by the build system.
   ------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rows_allowed.h"

static const bool row_allowed[8] = {
    /* 000 */ true,
    /* 001 */ true,
    /* 010 */ false,   /* forbidden horizontal pattern */
    /* 011 */ true,
    /* 100 */ true,
    /* 101 */ true,
    /* 110 */ true,
    /* 111 */ true
};

/* Pre‑compute transition masks for all (prev2, prev1) pairs. */
static void build_transition_masks(uint8_t mask[8][8])
{
    for (int p2 = 0; p2 < 8; ++p2) {
        for (int p1 = 0; p1 < 8; ++p1) {
            uint8_t m = 0;
            if (!row_allowed[p2] || !row_allowed[p1]) {
                mask[p2][p1] = 0;
                continue;
            }
            for (int c = 0; c < 8; ++c) {
                if (!row_allowed[c]) continue;
                if (rows_allowed((uint8_t)p2, (uint8_t)p1, (uint8_t)c))
                    m |= (1u << c);
            }
            mask[p2][p1] = m;
        }
    }
}

/* -------------------------------------------------------------
   Public function as required by the build system.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */

    /* immediate values for small n */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)49;

    /* transition masks (computed once) */
    static bool initialized = false;
    static uint8_t transition_mask[8][8];
    if (!initialized) {
        build_transition_masks(transition_mask);
        initialized = true;
    }

    /* DP arrays: dp_prev[prev2][prev1], dp_next[prev1][cur] */
    static seq_t dp_prev[8][8];
    static seq_t dp_next[8][8];

    /* initialize for first two rows */
    for (int p2 = 0; p2 < 8; ++p2)
        for (int p1 = 0; p1 < 8; ++p1)
            dp_prev[p2][p1] = (row_allowed[p2] && row_allowed[p1]) ? (seq_t)1 : (seq_t)0;

    /* iterate from row 3 to n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int p2 = 0; p2 < 8; ++p2) {
            for (int p1 = 0; p1 < 8; ++p1) {
                seq_t ways = dp_prev[p2][p1];
                if (ways == (seq_t)0) continue;
                uint8_t mask = transition_mask[p2][p1];
                while (mask) {
                    int cur = __builtin_ctz(mask);
                    mask &= mask - 1;                  /* clear lowest set bit */
                    dp_next[p1][cur] += ways;
                }
            }
        }
        /* swap dp_prev and dp_next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp_prev[i][j] = dp_next[i][j];
    }

    /* sum all states for final answer */
    seq_t total = (seq_t)0;
    for (int p2 = 0; p2 < 8; ++p2)
        for (int p1 = 0; p1 < 8; ++p1)
            total += dp_prev[p2][p1];

    return total;
}
