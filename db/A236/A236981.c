/*
 * A236981  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   subblock_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A236981: Number of (n+1)×3 arrays over {0,1,2,3} such that
   for every 2×2 subblock the sum of (maximum + upper median + lower
   median) is identical.  This implementation builds, once, the
   adjacency lists for each possible sum s ∈ {0,…,9}, then runs a
   simple dynamic‑programming walk over the graph for n steps.
   The function works for any seq_t/idx_t supplied by the build. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/subblock_sum.h"

#define NUM_ROWS 64          /* number of possible 3‑tuples 0..3 (4³) */
#define NUM_S    10          /* sums 0..9 */

static bool built = false;
static uint8_t rows[NUM_ROWS][3];
static uint8_t adj_next[NUM_S][NUM_ROWS][NUM_ROWS];
static uint8_t adj_deg[NUM_S][NUM_ROWS];

/* initialise the global adjacency tables once */
static void build_adjacency(void)
{
    if (built) return;
    /* decode all 3‑tuples into base‑4 digits */
    for (int p = 0; p < NUM_ROWS; ++p) {
        rows[p][0] = (p >> 4) & 3;
        rows[p][1] = (p >> 2) & 3;
        rows[p][2] = p & 3;
    }
    /* build adjacency lists */
    for (int s = 0; s < NUM_S; ++s)
        for (int p = 0; p < NUM_ROWS; ++p)
            adj_deg[s][p] = 0;

    for (int p = 0; p < NUM_ROWS; ++p) {
        const uint8_t *rp = rows[p];
        for (int q = 0; q < NUM_ROWS; ++q) {
            const uint8_t *rq = rows[q];
            int s0 = subblock_sum(rp, rq, 0);
            int s1 = subblock_sum(rp, rq, 1);
            if (s0 == s1) {
                uint8_t s = (uint8_t)s0;          /* 0 ≤ s ≤ 9 */
                uint8_t d = adj_deg[s][p]++;
                adj_next[s][p][d] = (uint8_t)q;
            }
        }
    }
    built = true;
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, not used here */

    build_adjacency();

    seq_t total = (seq_t)0;

    /* temporary storage for dynamic programming */
    seq_t dp_cur[NUM_ROWS];
    seq_t dp_next[NUM_ROWS];

    for (int s = 0; s < NUM_S; ++s) {
        /* initial state: length = 1 row (no steps yet) */
        for (int i = 0; i < NUM_ROWS; ++i) dp_cur[i] = (seq_t)1;

        /* perform n steps to reach n+1 rows */
        for (idx_t step = 1; step <= n; ++step) {
            /* zero the next array */
            for (int i = 0; i < NUM_ROWS; ++i) dp_next[i] = (seq_t)0;

            /* propagate over edges of sum s */
            for (int p = 0; p < NUM_ROWS; ++p) {
                uint8_t deg = adj_deg[s][p];
                seq_t val = dp_cur[p];
                for (int k = 0; k < deg; ++k) {
                    uint8_t q = adj_next[s][p][k];
                    dp_next[q] += val;
                }
            }
            /* copy dp_next → dp_cur for next iteration */
            for (int i = 0; i < NUM_ROWS; ++i) dp_cur[i] = dp_next[i];
        }

        /* sum all final states for this sum s */
        seq_t sum_s = (seq_t)0;
        for (int i = 0; i < NUM_ROWS; ++i) sum_s += dp_cur[i];
        total += sum_s;
    }

    return total;
}
