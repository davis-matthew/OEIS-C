/*
 * A202432  Number of (n+2)X7 binary arrays avoiding patterns 000 and 001 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×7 binary arrays avoiding patterns 000 and
 *            001 in rows, columns and NW‑to‑SE diagonals. Computed via
 *            dynamic programming over rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

#define ROWS 128
#define STATE_COUNT (ROWS * ROWS)

static bool    valid_row[ROWS];
static uint64_t trans_low[STATE_COUNT];
static uint64_t trans_high[STATE_COUNT];
static bool    precomp_done = false;

/* Check whether a row pattern of width 7 avoids 000 and 001 horizontally. */
static bool
row_ok(int pat)
{
    for (int j = 0; j <= 4; ++j) {
        int b0 = (pat >> j)     & 1;
        int b1 = (pat >> (j+1)) & 1;
        int b2 = (pat >> (j+2)) & 1;
        if ((b0 == 0 && b1 == 0 && b2 == 0) ||
            (b0 == 0 && b1 == 0 && b2 == 1))
            return false;
    }
    return true;
}

/* Check vertical (columns) and diagonal (NW‑to‑SE) constraints
   for a transition a,b -> c.  Assumes a,b,c are rows that are
   individually horizontally valid. */
static bool
transition_ok(int a, int b, int c)
{
    /* Columns */
    for (int j = 0; j < 7; ++j) {
        int aa = (a >> j) & 1;
        int bb = (b >> j) & 1;
        int cc = (c >> j) & 1;
        if ((aa == 0 && bb == 0 && cc == 0) ||
            (aa == 0 && bb == 0 && cc == 1))
            return false;
    }
    /* NW‑to‑SE diagonals (require j >= 2) */
    for (int j = 2; j < 7; ++j) {
        int aa = (a >> (j-2)) & 1;
        int bb = (b >> (j-1)) & 1;
        int cc = (c >> j)     & 1;
        if ((aa == 0 && bb == 0 && cc == 0) ||
            (aa == 0 && bb == 0 && cc == 1))
            return false;
    }
    return true;
}

/* Pre‑compute valid rows and transition masks.  This is done once. */
static void
precompute(void)
{
    if (precomp_done) return;

    for (int pat = 0; pat < ROWS; ++pat)
        valid_row[pat] = row_ok(pat);

    for (int a = 0; a < ROWS; ++a) {
        if (!valid_row[a]) continue;
        for (int b = 0; b < ROWS; ++b) {
            if (!valid_row[b]) continue;
            int idx = (a << 7) | b;
            uint64_t low = 0, high = 0;
            for (int c = 0; c < ROWS; ++c) {
                if (!valid_row[c]) continue;
                if (transition_ok(a, b, c)) {
                    if (c < 64) low  |= (uint64_t)1 << c;
                    else         high |= (uint64_t)1 << (c - 64);
                }
            }
            trans_low[idx]  = low;
            trans_high[idx] = high;
        }
    }
    precomp_done = true;
}

/* Main function required by the test harness. */
seq_t
a(idx_t n)
{
    precompute();

    /* Allocate DP arrays */
    seq_t *dp_prev = alloc_seq(STATE_COUNT);
    seq_t *dp_curr = alloc_seq(STATE_COUNT);
    memset(dp_prev, 0, STATE_COUNT * sizeof(seq_t));

    /* Initial two rows (height = 2) – any pair of horizontally valid rows */
    for (int a = 0; a < ROWS; ++a) {
        if (!valid_row[a]) continue;
        for (int b = 0; b < ROWS; ++b) {
            if (!valid_row[b]) continue;
            int idx = (a << 7) | b;
            dp_prev[idx] = 1;
        }
    }

    /* Add n additional rows to reach total height n+2 */
    for (idx_t step = 0; step < n; ++step) {
        memset(dp_curr, 0, STATE_COUNT * sizeof(seq_t));
        for (int idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t cnt = dp_prev[idx];
            if (cnt == 0) continue;
            int a = idx >> 7;
            int b = idx & 127;
            uint64_t low  = trans_low[idx];
            uint64_t high = trans_high[idx];

            while (low) {
                int bit = __builtin_ctzll(low);
                low &= low - 1;
                int c = bit;
                int new_idx = (b << 7) | c;
                dp_curr[new_idx] += cnt;
            }
            while (high) {
                int bit = __builtin_ctzll(high);
                high &= high - 1;
                int c = 64 + bit;
                int new_idx = (b << 7) | c;
                dp_curr[new_idx] += cnt;
            }
        }
        /* Swap DP arrays */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Sum all states to get the total number of arrays */
    seq_t total = 0;
    for (int idx = 0; idx < STATE_COUNT; ++idx)
        total += dp_prev[idx];

    free_seq(dp_prev);
    free_seq(dp_curr);
    return total;
}
