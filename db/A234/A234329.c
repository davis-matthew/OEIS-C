/*
 * A234329  Number of (n+1) X (4+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts (n+1)×5 arrays over {0,1,2,3} where every 2×2 subblock satisfies
   |(a+d)-(b+c)| = 1.  The solution is computed by dynamic programming
   on a graph of 4^5 = 1024 states (rows of length 5).  The transition
   graph is built once, then the number of paths of length n is accumulated. */
#include <stdlib.h>
#include <stdbool.h>

#define STATES 1024          /* 4^5 */
#define MAX_EDGES (STATES*STATES)

/* State encoding: 5 base‑4 digits 0..3 */
static uint8_t state_digits[STATES][5];
/* Adjacency: neighbor_start[s] = start index in neighbor_edges for state s */
static int neighbor_start[STATES + 1];
static int neighbor_edges[MAX_EDGES];

/* Build state digits and adjacency once. */
static void build_adjacency(void)
{
    static bool built = false;
    if (built) return;
    built = true;

    /* Pre‑compute digits for each state. */
    for (int s = 0; s < STATES; ++s) {
        int tmp = s;
        for (int i = 4; i >= 0; --i) {
            state_digits[s][i] = (uint8_t)(tmp & 3);
            tmp >>= 2;
        }
    }

    /* Count neighbors per state. */
    int edge_count[STATES];
    for (int a = 0; a < STATES; ++a) {
        int cnt = 0;
        const uint8_t *da = state_digits[a];
        for (int b = 0; b < STATES; ++b) {
            const uint8_t *db = state_digits[b];
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                int diff = (int)(da[j] + db[j + 1]) - (int)(da[j + 1] + db[j]);
                if (diff != 1 && diff != -1) { ok = false; break; }
            }
            if (ok) ++cnt;
        }
        edge_count[a] = cnt;
    }

    /* Prefix sums for adjacency storage. */
    int total = 0;
    for (int a = 0; a < STATES; ++a) {
        neighbor_start[a] = total;
        total += edge_count[a];
    }
    neighbor_start[STATES] = total;    /* sentinel */

    /* Fill adjacency list. */
    for (int a = 0; a < STATES; ++a) {
        const uint8_t *da = state_digits[a];
        int pos = neighbor_start[a];
        for (int b = 0; b < STATES; ++b) {
            const uint8_t *db = state_digits[b];
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                int diff = (int)(da[j] + db[j + 1]) - (int)(da[j + 1] + db[j]);
                if (diff != 1 && diff != -1) { ok = false; break; }
            }
            if (ok) neighbor_edges[pos++] = b;
        }
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Ensure adjacency is ready. */
    build_adjacency();

    /* Static DP arrays to avoid dynamic allocation of user types. */
    static seq_t dp[STATES];
    static seq_t next[STATES];

    /* Initialization: each possible first row is a start state. */
    const seq_t one = (seq_t)1;
    for (int s = 0; s < STATES; ++s) dp[s] = one;

    /* Perform n transitions (array height = n+1). */
    for (idx_t step = 1; step <= n; ++step) {
        /* Reset next array. */
        for (int s = 0; s < STATES; ++s) next[s] = (seq_t)0;

        /* Transition counts. */
        for (int a = 0; a < STATES; ++a) {
            seq_t val = dp[a];
            if (val == (seq_t)0) continue;
            int start = neighbor_start[a];
            int end   = neighbor_start[a + 1];
            for (int p = start; p < end; ++p) {
                int b = neighbor_edges[p];
                next[b] += val;
            }
        }

        /* Swap dp and next. */
        for (int s = 0; s < STATES; ++s) dp[s] = next[s];
    }

    /* Sum all final state counts. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATES; ++s) result += dp[s];

    return result;
}
