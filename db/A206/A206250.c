/*
 * A206250  Number of (n+1) X 4 0..3 arrays with every 2 X 2 subblock having
 *          zero permanent.
 * https://oeis.org/A206250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×4 arrays with entries 0..3 such that every 2×2 subblock has zero permanent.
 * Computed by dynamic programming over 256 possible rows; adjacency derived from the permanent condition.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- adjacency precomputation ---------- */
static bool adj_inited = false;
static uint16_t adj_start[257];          /* start index for each row's adjacency list */
static uint16_t adj_edges[256 * 256];    /* concatenated adjacency lists (max 65536 edges) */

/* decode a state (0..255) into its four base‑4 digits */
static void decode_state(uint16_t s, uint8_t d[4]) {
    d[3] = s % 4; s /= 4;
    d[2] = s % 4; s /= 4;
    d[1] = s % 4; s /= 4;
    d[0] = s % 4;
}

/* test compatibility of two rows */
static bool compatible(const uint8_t a[4], const uint8_t b[4]) {
    /* 2×2 block at columns (0,1) */
    if (a[0] != 0 && b[1] != 0) return false;
    if (a[1] != 0 && b[0] != 0) return false;
    /* 2×2 block at columns (1,2) */
    if (a[1] != 0 && b[2] != 0) return false;
    if (a[2] != 0 && b[1] != 0) return false;
    /* 2×2 block at columns (2,3) */
    if (a[2] != 0 && b[3] != 0) return false;
    if (a[3] != 0 && b[2] != 0) return false;
    return true;
}

/* initialise adjacency data structures once */
static void init_adjacency(void) {
    if (adj_inited) return;
    uint8_t d1[4], d2[4];
    size_t next_idx = 0;
    adj_start[0] = 0;
    for (uint16_t i = 0; i < 256; ++i) {
        decode_state(i, d1);
        for (uint16_t j = 0; j < 256; ++j) {
            decode_state(j, d2);
            if (compatible(d1, d2)) {
                adj_edges[next_idx++] = (uint16_t)j;
            }
        }
        adj_start[i + 1] = (uint16_t)next_idx;
    }
    adj_inited = true;
}

/* ---------- dynamic programming implementation ---------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    init_adjacency();

    /* arrays for current and next row counts */
    static seq_t cur[256];
    static seq_t next[256];

    /* base case: first row can be any of the 256 states */
    for (size_t i = 0; i < 256; ++i) cur[i] = 1;

    /* iterate for each additional row up to n rows total (n+1 rows array) */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next array */
        for (size_t i = 0; i < 256; ++i) next[i] = 0;

        /* transition */
        for (uint16_t i = 0; i < 256; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            uint16_t start = adj_start[i];
            uint16_t end   = adj_start[i + 1];
            for (uint16_t idx = start; idx < end; ++idx) {
                uint16_t j = adj_edges[idx];
                next[j] += val;
            }
        }

        /* if we have reached the desired number of rows, sum and return */
        if (step == n) {
            seq_t total = 0;
            for (size_t i = 0; i < 256; ++i) total += next[i];
            return total;
        }

        /* prepare for next iteration: cur = next */
        for (size_t i = 0; i < 256; ++i) cur[i] = next[i];
    }

    /* unreachable but required for compilation */
    return 0;
}
