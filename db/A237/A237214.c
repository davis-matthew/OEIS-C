/*
 * A237214  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of (n+1)×4 arrays of digits 0..3 in which all 2×2 sub‑blocks
 * satisfy f = constant, where f = max + upper_median − lower_median − min.
 * The table is indexed by row number (0..n) and the column vector (0..255).
 * The function returns a(idx_t n) for n ≥ 1. */
#include <stdint.h>
#include <stddef.h>

/* Local helper: compute the value of f for a 2×2 block. */
static uint8_t f_val(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    uint8_t mn = 3, smn = 3, mx = 0, smx = 0;
    uint8_t arr[4] = { a, b, c, d };
    for (int k = 0; k < 4; ++k) {
        uint8_t x = arr[k];
        if (x < mn) { smn = mn; mn = x; }
        else if (x < smn) smn = x;
        if (x > mx) { smx = mx; mx = x; }
        else if (x > smx) smx = x;
    }
    return mx + smx - smn - mn;
}

/* Adjacency tables for all possible values of f (0..6). */
static uint8_t  adj_deg[7][256];
static uint16_t adj_nei[7][256][256];
static int      adj_init = 0;

/* Pre‑computed row vectors for all 256 possible rows. */
static uint8_t rows_vec[256][4];

/* Build the adjacency lists once. */
static void build_adjacency(void)
{
    if (adj_init) return;

    /* Fill rows_vec with Base‑4 representations of 0..255. */
    for (uint16_t r = 0; r < 256; ++r) {
        rows_vec[r][0] = (uint8_t)((r >> 0) & 3);
        rows_vec[r][1] = (uint8_t)((r >> 2) & 3);
        rows_vec[r][2] = (uint8_t)((r >> 4) & 3);
        rows_vec[r][3] = (uint8_t)((r >> 6) & 3);
    }

    for (int C = 0; C <= 6; ++C) {
        for (uint16_t p = 0; p < 256; ++p) {
            adj_deg[C][p] = 0;
            for (uint16_t q = 0; q < 256; ++q) {
                int ok = 1;
                for (int j = 0; j <= 2; ++j) {
                    uint8_t val = f_val(rows_vec[p][j], rows_vec[p][j+1],
                                       rows_vec[q][j], rows_vec[q][j+1]);
                    if (val != (uint8_t)C) { ok = 0; break; }
                }
                if (ok) {
                    uint16_t d = adj_deg[C][p];
                    adj_nei[C][p][d++] = (uint16_t)q;
                    adj_deg[C][p] = d;
                }
            }
        }
    }
    adj_init = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1). */
    build_adjacency();

    /* Dynamic programming arrays for one row value. */
    seq_t dp1[256];
    seq_t dp2[256];
    seq_t *from = dp1;
    seq_t *to   = dp2;

    seq_t total = 0;

    for (int C = 0; C <= 6; ++C) {
        /* Initial state: any first row is allowed. */
        for (int i = 0; i < 256; ++i) from[i] = 1;

        /* Perform n transitions (to generate n+1 rows). */
        for (idx_t step = 0; step < n; ++step) {
            for (int i = 0; i < 256; ++i) to[i] = 0;

            for (int p = 0; p < 256; ++p) {
                seq_t val = from[p];
                if (val == 0) continue;
                uint8_t d = adj_deg[C][p];
                for (uint8_t k = 0; k < d; ++k) {
                    int q = adj_nei[C][p][k];
                    to[q] += val;
                }
            }
            /* Swap the DP buffers. */
            seq_t *tmp = from;
            from = to;
            to   = tmp;
        }

        /* Sum numbers of arrays ending in any row. */
        seq_t sum = 0;
        for (int i = 0; i < 256; ++i) sum += from[i];
        total += sum;
    }
    return total;
}
