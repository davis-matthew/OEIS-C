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
 * Helpers:   build_cols, build_transition
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
#include "../utils/build_cols.h"
#include "../utils/build_transition.h"

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
        dp_prev = (seq_t *)malloc(dp_size * sizeof(seq_t));
        dp_next = (seq_t *)malloc(dp_size * sizeof(seq_t));

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
