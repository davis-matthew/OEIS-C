/*
 * A205091  Number of (n+1)X3 0..3 arrays with the number of rightwards and
 *          downwards edge increases in each 2X2 subblock equal to the number
 *          in all its horizontal and vertical neighbors.
 * https://oeis.org/A205091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   compute_subblock_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A205091: Number of (n+1)×3 arrays of digits 0..3 such that
 * for every 2×2 subblock the number of increasing (rightwards or downwards)
 * edges equals the same number for all of its horizontal and vertical
 * neighbours.  This implementation uses dynamic programming over row
 * configurations; for each possible common sum s∈{0,…,4} we compute
 * transitions between row states that preserve that sum for the two
 * subblocks adjacent in the same row pair.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/compute_subblock_sum.h"

/* Pre‑computed adjacency lists for each allowed sum s (0..4). */
/* For a given sum s and starting row state i (0..63) the list contains all
 * row states j (0..63) such that the 2×2 subblocks formed by rows i and j
 * (columns 0–1 and 1–2) both have precisely s increasing edges. */
static unsigned char neighbor_list[5][64][64];
static unsigned char neighbor_cnt[5][64];

static void init(void)
{
    /* Build the adjacency lists only once. */
    static int ready = 0;
    if (ready) return;
    for (int s = 0; s < 5; ++s) {
        for (int i = 0; i < 64; ++i) {
            neighbor_cnt[s][i] = 0;
            for (int j = 0; j < 64; ++j) {
                int sum0, sum1;
                compute_subblock_sum(i, j, &sum0, &sum1);
                if (sum0 == sum1 && sum0 == s) {
                    neighbor_list[s][i][neighbor_cnt[s][i]++] = (unsigned char) j;
                }
            }
        }
    }
    ready = 1;
}

/* Main sequence function.  Indexing follows the OEIS offset (a(1) is first term) */
seq_t a(idx_t n)
{
    init();
    /* dp[s][i] – number of ways to reach row state i with all subblocks
     * up to the current row having common sum s.  dp_next is the same
     * for the next row.  All values are stored in a seq_t array. */
    seq_t dp[5][64];
    seq_t dp_next[5][64];

    /* Initialise: for the very first row there is no restriction, thus
     * each state is admissible for every sum value. */
    for (int s = 0; s < 5; ++s)
        for (int i = 0; i < 64; ++i)
            dp[s][i] = (seq_t)1;

    /* We have n transitions between consecutive rows (total rows = n+1). */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero dp_next properly without memsetting struct data. */
        for (int s = 0; s < 5; ++s)
            for (int i = 0; i < 64; ++i)
                dp_next[s][i] = (seq_t)0;

        for (int s = 0; s < 5; ++s) {
            for (int i = 0; i < 64; ++i) {
                seq_t val = dp[s][i];
                if (val == (seq_t)0) continue;
                unsigned char *nbr = neighbor_list[s][i];
                int cnt = neighbor_cnt[s][i];
                for (int k = 0; k < cnt; ++k) {
                    int j = nbr[k];
                    dp_next[s][j] += val;
                }
            }
        }
        /* Move to the next row. */
        for (int s = 0; s < 5; ++s)
            for (int i = 0; i < 64; ++i)
                dp[s][i] = dp_next[s][i];
    }

    /* Sum all counts for every resulting state and sum value. */
    seq_t total = (seq_t)0;
    for (int s = 0; s < 5; ++s)
        for (int i = 0; i < 64; ++i)
            total += dp[s][i];

    return total;
}
