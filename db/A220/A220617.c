/*
 * A220617  Number of ways to reciprocally link elements of an nX4 array
 *          either to themselves or to exactly one horizontal, diagonal and
 *          antidiagonal neighbor.
 * https://oeis.org/A220617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by a transfer‑matrix recurrence. The graph of an n×4
 *            grid (horizontal and diagonal connections) has only 16 possible
 *            states for each row. The transition matrix T[16][16] gives the
 *            number of ways to transform a row with incoming state `in` to a
 *            row with outgoing state `out`. Let `dp` be a...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>          /* for memset */

/* The types seq_t and idx_t are supplied by the build.  The functions
   below only use +, -, *, /, %, comparisons and bitwise operators
   as required. */

static seq_t T[16][16] = {0};          /* transition matrix */

/* Recursive generation of transitions for a single row. */
static void dfs(uint8_t used, uint8_t out_mask, uint8_t in_mask)
{
    /* all four vertices processed */
    if (used == 0x0F) {
        T[in_mask][out_mask] = T[in_mask][out_mask] + (seq_t)1;
        return;
    }
    /* find the first free (unused) vertex */
    uint8_t c = 0;
    while (c < 4 && (used & (1u << c))) ++c;
    /* (c is guaranteed to be < 4) */

    /* 1. Loop on this vertex */
    dfs(used | (1u << c), out_mask, in_mask);

    /* 2. Horizontal pair with the right neighbour */
    if (c < 3 && !(used & (1u << (c + 1)))) {
        dfs(used | (1u << c) | (1u << (c + 1)), out_mask, in_mask);
    }

    /* 3. Cross‑row to the right neighbour of the next row */
    if (c < 3 && !(out_mask & (1u << (c + 1)))) {
        dfs(used | (1u << c), out_mask | (1u << (c + 1)), in_mask);
    }

    /* 4. Cross‑row to the left neighbour of the next row */
    if (c > 0 && !(out_mask & (1u << (c - 1)))) {
        dfs(used | (1u << c), out_mask | (1u << (c - 1)), in_mask);
    }
}

/* Initialise the transition matrix once. */
static void init_transitions(void)
{
    /* Already zeroed at compile time; guard against multiple calls. */
    static int inited = 0;
    if (inited) return;
    inited = 1;

    for (uint8_t in = 0; in < 16; ++in) {
        dfs(in, 0, in);
    }
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    init_transitions();          /* ensure T is ready */

    /* dp arrays: 16 states each */
    static seq_t dp[16]      = {0};
    static seq_t dp_next[16] = {0};

    /* initialise dp for zero rows */
    memset(dp, 0, sizeof(dp));
    dp[0] = (seq_t)1;

    /* process all rows up to n */
    for (idx_t row = 1; row <= n; ++row) {
        memset(dp_next, 0, sizeof(dp_next));
        for (uint8_t in = 0; in < 16; ++in) {
            if (dp[in] == (seq_t)0) continue;
            seq_t val_in = dp[in];
            for (uint8_t out = 0; out < 16; ++out) {
                if (T[in][out] == (seq_t)0) continue;
                seq_t prod = val_in * (seq_t)T[in][out];
                dp_next[out] = dp_next[out] + prod;
            }
        }
        /* copy back for next iteration */
        memcpy(dp, dp_next, sizeof(dp));
    }

    return dp[0];   /* no pending connections after the last row */
}
