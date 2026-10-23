/*
 * A184193  Half the number of (n+1)X6 binary arrays with no 2X2 subblock
 *          containing exactly one 1.
 * https://oeis.org/A184193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   eval_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A184193
 * a(n) = ½ * the number of (n+1)×6 binary arrays with no 2×2 subblock
 * containing exactly one 1.
 * Computation is performed by dynamic programming on row states.
 * Each row is a 6‑bit string; adjacency between rows is allowed when
 * all five overlapping 2×2 blocks do not contain exactly one 1.
 * For n ≥ 1, the number of arrays is obtained by iterating n transitions
 * from an initial vector containing one for each possible first row.
 * The result is the sum of the final vector divided by two.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/eval_ok.h"

/* Pre‑computed adjacency bitmask for each of the 64 row states. */
static uint64_t adj_mask[64];
static bool adj_ready = false;

static void init_adj(void)
{
    for (int s = 0; s < 64; ++s) {
        uint64_t m = 0;
        for (int t = 0; t < 64; ++t) {
            if (eval_ok(s, t))
                m |= ((uint64_t)1 << t);
        }
        adj_mask[s] = m;
    }
    adj_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is unused */
    if (!adj_ready) init_adj();

    /* Current counts of sequences ending in each state */
    seq_t cur[64];
    seq_t nxt[64];
    for (int i = 0; i < 64; ++i) cur[i] = (seq_t)1;

    /* Iterate n transitions (n rows beyond the first) */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i) nxt[i] = (seq_t)0;
        for (int s = 0; s < 64; ++s) {
            if (cur[s] == (seq_t)0) continue;
            uint64_t m = adj_mask[s];
            while (m) {
                int t = __builtin_ctzll(m);   /* next state index */
                m &= m - 1;                   /* clear lowest set bit */
                nxt[t] += cur[s];
            }
        }
        /* swap cur and nxt */
        for (int i = 0; i < 64; ++i) cur[i] = nxt[i];
    }

    /* Sum over all ending states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i) total += cur[i];

    /* Return half the total count */
    return total / (seq_t)2;
}
