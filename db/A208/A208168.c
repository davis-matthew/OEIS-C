/*
 * A208168  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 207 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A208168: Number of 7×n binary arrays avoiding horizontally 000 and 101
 * and vertically 001 and 010.
 * The code builds all admissible column patterns, precomputes a transition
 * adjacency list for the 3‑column horizontal constraints, and applies a
 * simple DP that runs in O(n · (#edges)) time. 
 * No memoisation is required – each call works independently and is
 * fast enough for n up to 10 000. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Build the list of admissible 7‑bit columns (vertical constraints). */
static void build_cols(uint8_t **cols_out, size_t *S_out)
{
    uint8_t tmp[128];
    size_t ct = 0;

    for (uint32_t col = 0; col < (1u << 7); ++col) {
        bool bad = false;
        for (int i = 0; i <= 4; ++i) {
            uint8_t b0 = (col >> i) & 1u;
            uint8_t b1 = (col >> (i + 1)) & 1u;
            uint8_t b2 = (col >> (i + 2)) & 1u;
            if ((b0 == 0u && b1 == 0u && b2 == 1u) ||
                (b0 == 0u && b1 == 1u && b2 == 0u)) {
                bad = true;
                break;
            }
        }
        if (!bad) tmp[ct++] = (uint8_t)col;
    }

    *S_out = ct;
    *cols_out = (uint8_t *)malloc(ct * sizeof(uint8_t));
    for (size_t i = 0; i < ct; ++i) (*cols_out)[i] = tmp[i];
}

/* Precompute, for each pair of columns (a,b), the list of columns c
 * that together with a and b satisfy the horizontal constraints.          */
static void build_transition(const uint8_t *cols, size_t S,
                             uint32_t **adj_start_out, uint8_t **next_col_out,
                             size_t *total_edges_out)
{
    size_t num_pairs = S * S;
    size_t *outdeg = (size_t *)malloc(num_pairs * sizeof(size_t));
    size_t total_edges = 0;

    /* First pass: count outdegrees. */
    for (size_t a = 0; a < S; ++a) {
        for (size_t b = 0; b < S; ++b) {
            size_t idx = a * S + b;
            size_t cnt = 0;
            for (size_t c = 0; c < S; ++c) {
                bool ok = true;
                for (int r = 0; r < 7; ++r) {
                    uint8_t x = (cols[a] >> r) & 1u;
                    uint8_t y = (cols[b] >> r) & 1u;
                    uint8_t z = (cols[c] >> r) & 1u;
                    if ((x == 0u && y == 0u && z == 0u) ||
                        (x == 1u && y == 0u && z == 1u)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) ++cnt;
            }
            outdeg[idx] = cnt;
            total_edges += cnt;
        }
    }

    /* Allocate adjacency array. */
    uint32_t *adj_start = (uint32_t *)malloc((num_pairs + 1) * sizeof(uint32_t));
    uint8_t *next_col   = (uint8_t *)malloc(total_edges * sizeof(uint8_t));

    /* Second pass: fill adjacency. */
    adj_start[0] = 0;
    size_t pos = 0;
    for (size_t a = 0; a < S; ++a) {
        for (size_t b = 0; b < S; ++b) {
            size_t idx = a * S + b;
            for (size_t c = 0; c < S; ++c) {
                bool ok = true;
                for (int r = 0; r < 7; ++r) {
                    uint8_t x = (cols[a] >> r) & 1u;
                    uint8_t y = (cols[b] >> r) & 1u;
                    uint8_t z = (cols[c] >> r) & 1u;
                    if ((x == 0u && y == 0u && z == 0u) ||
                        (x == 1u && y == 0u && z == 1u)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    next_col[pos++] = (uint8_t)c;
                }
            }
            adj_start[idx + 1] = (uint32_t)pos;
        }
    }

    *adj_start_out = adj_start;
    *next_col_out   = next_col;
    *total_edges_out = total_edges;
    free(outdeg);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* One-time global state. */
    static bool init = false;
    static size_t S = 0;
    static uint8_t *cols = NULL;
    static uint32_t *adj_start = NULL;
    static uint8_t *next_col   = NULL;

    static seq_t *dp_prev = NULL;
    static seq_t *dp_next = NULL;
    static size_t dp_size = 0;

    if (!init) {
        /* Build columns. */
        build_cols(&cols, &S);

        /* Precompute transitions. */
        size_t total_edges;
        build_transition(cols, S, &adj_start, &next_col, &total_edges);

        /* Allocate DP arrays. */
        dp_size = S * S;
        dp_prev = mem_new(seq_t, dp_size);
        dp_next = mem_new(seq_t, dp_size);

        init = true;
    }

    /* Small n cases. */
    if (n == 1) {
        return (seq_t)S;
    }
    if (n == 2) {
        return (seq_t)S * (seq_t)S;
    }

    /* Initialise dp_prev for length 2 (all pairs allowed). */
    for (size_t i = 0; i < dp_size; ++i) dp_prev[i] = (seq_t)1;

    /* Iterate for lengths 3 .. n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero dp_next. */
        for (size_t i = 0; i < dp_size; ++i) dp_next[i] = (seq_t)0;

        /* Transition using precomputed adjacency. */
        for (size_t a = 0; a < S; ++a) {
            for (size_t b = 0; b < S; ++b) {
                size_t p = a * S + b;
                seq_t val = dp_prev[p];
                if (val == (seq_t)0) continue;
                uint32_t start = adj_start[p];
                uint32_t end   = adj_start[p + 1];
                for (uint32_t idx = start; idx < end; ++idx) {
                    uint8_t c = next_col[idx];
                    dp_next[b * S + c] += val;
                }
            }
        }

        /* Swap dp_prev and dp_next for next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    /* Sum all entries of dp_prev for the final length. */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < dp_size; ++i) total += dp_prev[i];
    return total;
}
