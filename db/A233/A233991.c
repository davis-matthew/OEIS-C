/*
 * A233991  Number of (n+1)X(2+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 18.
 * https://oeis.org/A233991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of (n+1)×3 arrays with entries 0..6 such that every 2×2 block has the sum
 * of the absolute values of all six edge and diagonal differences equal to 18.
 * The values are obtained by dynamic programming on a graph with 343 vertices
 * (each 3‑digit row).  The algorithm runs in O(n) time with a tiny constant. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define STATES 343
#define MAX_EDGES 120000

/* adjacency lists stored in CSR format */
static unsigned int head[STATES];
static unsigned int next_edge[MAX_EDGES];
static unsigned int to_state[MAX_EDGES];
static unsigned int edge_count = 0;

static unsigned char row_digits[STATES][3];
static bool adjacency_built = false;

/* Build adjacency lists once */
static void build_adjacency(void)
{
    if (adjacency_built) return;
    /* decode each state into its three digits */
    for (unsigned int s = 0; s < STATES; ++s) {
        unsigned int t = s;
        row_digits[s][0] = t % 7; t /= 7;
        row_digits[s][1] = t % 7; t /= 7;
        row_digits[s][2] = t;
        head[s] = (unsigned int)-1;
    }

    /* test all pairs of rows */
    for (unsigned int s1 = 0; s1 < STATES; ++s1) {
        const unsigned char *a = row_digits[s1];
        for (unsigned int s2 = 0; s2 < STATES; ++s2) {
            const unsigned char *b = row_digits[s2];
            bool ok = true;
            for (int j = 0; j < 2 && ok; ++j) {
                int s = 0;
                s += abs((int)a[j] - (int)a[j+1]);          /* top edge    */
                s += abs((int)b[j] - (int)b[j+1]);          /* bottom edge */
                s += abs((int)a[j] - (int)b[j]);            /* left edge   */
                s += abs((int)a[j+1] - (int)b[j+1]);        /* right edge  */
                s += abs((int)a[j] - (int)b[j+1]);          /* diag         */
                s += abs((int)a[j+1] - (int)b[j]);          /* diag         */
                if (s != 18) ok = false;
            }
            if (ok) {
                to_state[edge_count] = (unsigned int)s2;
                next_edge[edge_count] = head[s1];
                head[s1] = edge_count;
                ++edge_count;
            }
        }
    }
    adjacency_built = true;
}

/* DP arrays */
static seq_t dp[STATES];
static seq_t newdp[STATES];

seq_t a(idx_t n)
{
    /* n is number of 2x2 blocks, i.e. rows-1 */
    if (n == 0) return 0;            /* not defined for offset < 1 */
    build_adjacency();

    /* initial state: any first row allowed */
    for (unsigned int i = 0; i < STATES; ++i) dp[i] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (unsigned int i = 0; i < STATES; ++i) newdp[i] = 0;
        for (unsigned int s1 = 0; s1 < STATES; ++s1) {
            unsigned int e = head[s1];
            while (e != (unsigned int)-1) {
                unsigned int s2 = to_state[e];
                newdp[s2] += dp[s1];
                e = next_edge[e];
            }
        }
        for (unsigned int i = 0; i < STATES; ++i) dp[i] = newdp[i];
    }

    seq_t total = 0;
    for (unsigned int i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
