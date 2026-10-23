/*
 * A183307  Half the number of nX6 binary arrays with no element equal to a
 *          strict majority of its horizontal and vertical neighbors.
 * https://oeis.org/A183307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = half the number of n×6 binary arrays with no element
 *            equal to a strict majority of its horizontal and vertical
 *            neighbors. Implementation uses dynamic programming over rows
 *            with state comprising two consecutive rows
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define ROWS 64

/* Pre‑computed tables */
static bool top_ok[ROWS][ROWS];
static bool bottom_ok[ROWS][ROWS];
static uint8_t trans[ROWS][ROWS][ROWS];
static uint8_t trans_len[ROWS][ROWS];
static bool tables_initialized = false;

/* Check a single 6‑bit row for the local majority condition.  */
static inline bool row_valid(uint8_t row,
                             uint8_t up, uint8_t down,
                             bool has_up, bool has_down)
{
    for (int col = 0; col < 6; ++col) {
        uint8_t bit = (row >> col) & 1;
        int neigh = 0, equal = 0;

        /* left neighbor */
        if (col > 0) {
            ++neigh;
            if (((row >> (col - 1)) & 1) == bit) ++equal;
        }
        /* right neighbor */
        if (col < 5) {
            ++neigh;
            if (((row >> (col + 1)) & 1) == bit) ++equal;
        }
        /* up neighbor */
        if (has_up) {
            ++neigh;
            if (((up >> col) & 1) == bit) ++equal;
        }
        /* down neighbor */
        if (has_down) {
            ++neigh;
            if (((down >> col) & 1) == bit) ++equal;
        }

        if (equal * 2 > neigh) return false;  /* strict majority */
    }
    return true;
}

/* Build all required tables once. */
static void init_tables(void)
{
    if (tables_initialized) return;
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            top_ok[a][b] = row_valid((uint8_t)a, 0, (uint8_t)b, false, true);
            bottom_ok[b][a] = row_valid((uint8_t)b, (uint8_t)a, 0, true, false);

            /* transitions for middle rows */
            uint8_t *list = trans[a][b];
            uint8_t idx = 0;
            for (int c = 0; c < ROWS; ++c) {
                if (row_valid((uint8_t)b, (uint8_t)a, (uint8_t)c, true, true))
                    list[idx++] = (uint8_t)c;
            }
            trans_len[a][b] = idx;
        }
    }
    tables_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    /* Special case n = 1 (single row).  */
    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < ROWS; ++r)
            if (row_valid((uint8_t)r, 0, 0, false, false))
                total += (seq_t)1;
        return total / 2;   /* half of the counted arrays */
    }

    /* DP arrays for states (prev_row, cur_row) */
    seq_t cur[ROWS][ROWS];
    seq_t nxt[ROWS][ROWS];

    /* Initialization: only top row constraints satisfied */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            cur[a][b] = top_ok[a][b] ? (seq_t)1 : (seq_t)0;

    /* Process rows 2 .. n-1 (0‑based indexing) */
    for (idx_t step = 2; step < n; ++step) {
        /* zero the next array */
        for (int a = 0; a < ROWS; ++a)
            for (int b = 0; b < ROWS; ++b)
                nxt[a][b] = 0;

        /* transitions */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                seq_t val = cur[a][b];
                if (val == 0) continue;
                uint8_t *list = trans[a][b];
                uint8_t len   = trans_len[a][b];
                for (uint8_t t = 0; t < len; ++t) {
                    int c = list[t];
                    nxt[b][c] += val;
                }
            }
        }

        /* swap cur and nxt for next iteration */
        for (int a = 0; a < ROWS; ++a)
            for (int b = 0; b < ROWS; ++b)
                cur[a][b] = nxt[a][b];
    }

    /* Sum over valid bottom rows */
    seq_t total = 0;
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            if (bottom_ok[b][a])
                total += cur[a][b];

    return total / 2;   /* half of the counted arrays */
}
