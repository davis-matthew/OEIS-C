/*
 * A234723  Number of (n+1) X (3+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 7
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of order 16 (not used in this
 *            implementation). Computation is performed by dynamic programming
 *            with a transfer‑matrix on the 4 columns (rows of length 4). Each
 *            row is a 4‑tuple of digits 0..7. Two consecutive rows are
 *            compatible if for c=0,1,2 | (row1[c] + row2[c+1]) –...
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define STATES (8*8*8*8)   /* 4096 rows of length 4 */

static uint8_t row_digits[STATES][4];
static uint16_t adj[STATES][64];
static uint8_t adj_len[STATES];
static bool initialized = false;

/* Build all 4‑digit rows (base 8) and the compatibility graph. */
static void build_array(void)
{
    /* Pre‑compute digit decompositions of the 4096 states. */
    for (int i = 0; i < STATES; ++i) {
        int v = i;
        for (int d = 3; d >= 0; --d) {
            row_digits[i][d] = v & 7;
            v >>= 3;
        }
    }

    /* Build adjacency lists.  Each list is bounded by 64 entries,
       which is ample for the expected average degree (~1).     */
    for (int p = 0; p < STATES; ++p) {
        adj_len[p] = 0;
        for (int q = 0; q < STATES; ++q) {
            bool ok = true;
            for (int c = 0; c < 3 && ok; ++c) {
                int diff = (int)row_digits[p][c] + (int)row_digits[q][c+1]
                         - (int)row_digits[q][c] - (int)row_digits[p][c+1];
                if (diff != 7 && diff != -7)
                    ok = false;
            }
            if (ok) {
                if (adj_len[p] < 64)
                    adj[p][adj_len[p]++] = (uint16_t)q;
            }
        }
    }
}

/* Sequence generator. */
seq_t a(idx_t n)
{
    /* Ensure the graph is built only once. */
    if (!initialized) {
        build_array();
        initialized = true;
    }

    /* DP arrays for counts of sequences ending in each state. */
    static seq_t counts[STATES];
    static seq_t next_counts[STATES];

    /* Initial row – any of the 4096 rows is possible. */
    for (int i = 0; i < STATES; ++i)
        counts[i] = 1;

    /* We have (n+1) rows; after the first row we need n more. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero out next_counts. */
        for (int i = 0; i < STATES; ++i)
            next_counts[i] = 0;

        /* Transition: for each previous state add to compatible next states. */
        for (int p = 0; p < STATES; ++p) {
            seq_t cnt = counts[p];
            if (cnt == 0) continue;
            for (uint8_t k = 0; k < adj_len[p]; ++k) {
                uint16_t q = adj[p][k];
                next_counts[q] += cnt;
            }
        }

        /* Swap buffers. */
        for (int i = 0; i < STATES; ++i)
            counts[i] = next_counts[i];
    }

    /* Sum counts over all possible last rows to get the total. */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i)
        total += counts[i];

    return total;
}
