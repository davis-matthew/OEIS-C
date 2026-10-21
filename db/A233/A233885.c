/*
 * A233885  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 30,
 *          and no two adjacent values equal.
 * https://oeis.org/A233885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of (n+1)×2 arrays over {0,…,6} in which *
 *            (i) no two adjacent cells (horizontally or vertically) are
 *            equal, and * (ii) every 2×2 subblock has the sum of the squares
 *            of the edge * differences equal to 30. The function computes
 *            these values by * dynamic programming on a graph...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Enumerate the 42 allowed states: pairs (a,b) with a≠b, 0≤a,b≤6 */
enum { STATE_COUNT = 42 };

/* Storage for state values and adjacency lists (initialised once). */
static int a_val[STATE_COUNT];
static int b_val[STATE_COUNT];
static int neigh[STATE_COUNT][STATE_COUNT];
static int neigh_cnt[STATE_COUNT];
static int init_done = 0;

/* DP working arrays (static to avoid re‑allocation). */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

seq_t a(idx_t n)
{
    /* Return 0 for the undefined offset n = 0. */
    if (n == 0) return 0;

    /* One‑time construction of states and transition graph. */
    if (!init_done) {
        /* Enumerate all (a,b) with a≠b. */
        int idx = 0;
        for (int i = 0; i <= 6; ++i)
            for (int j = 0; j <= 6; ++j)
                if (i != j) {
                    a_val[idx] = i;
                    b_val[idx] = j;
                    ++idx;
                }

        /* Build adjacency lists. */
        for (int i = 0; i < STATE_COUNT; ++i) {
            int ai = a_val[i];
            int bi = b_val[i];
            int cnt = 0;
            for (int j = 0; j < STATE_COUNT; ++j) {
                int aj = a_val[j];
                int bj = b_val[j];

                /* Vertical adjacency must differ. */
                if (ai == aj || bi == bj) continue;

                /* Check the 2×2 block condition. */
                int d1 = ai - bi;
                int d2 = aj - bj;
                int d3 = ai - aj;
                int d4 = bi - bj;
                int sq_sum = d1*d1 + d2*d2 + d3*d3 + d4*d4;
                if (sq_sum == 30) {
                    neigh[i][cnt++] = j;
                }
            }
            neigh_cnt[i] = cnt;
        }
        init_done = 1;
    }

    /* Initialise DP: each allowed first row is valid. */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp_cur[i] = 1;

    /* Perform n transitions to obtain (n+1) rows. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = 0;

        for (int i = 0; i < STATE_COUNT; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            for (int k = 0; k < neigh_cnt[i]; ++k) {
                int j = neigh[i][k];
                dp_next[j] += val;
            }
        }

        /* Copy next to current for the next iteration. */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum all final states to obtain the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp_cur[i];

    return total;
}
