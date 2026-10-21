/*
 * A237805  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A237805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   subblock_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: DP over rows with adjacency matrix for each possible
   constant K ∈ {0,1,2,3,4}.  For each K we pre‑compute which pairs of
   consecutive rows (each row is a 5‑tuple of values 0..2) satisfy
   that all four 2×2 blocks produced by the pair have the same
   value max+uppermedian−min=K.  Then we count walks of length n over
   this graph (n transitions between n+1 rows) by dynamic programming.
   The total is the sum over K of the number of such walks. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/subblock_f.h"

/* build of adjacency lists for each K is done once */
static bool built = false;
#define N_STATES 243           /* 3^5 possible rows */
#define MAX_K    5             /* K in [0,4] */
#define MAX_NEIGH 243

/* adjacency: for each K (0..4), for each source state s,
   list of target states.  adj_counts[K][s] gives number of
   valid targets.  Adjacent states are stored in adj[K][s][*]  */
static uint8_t  adj[MAX_K][N_STATES][MAX_NEIGH];
static uint8_t  adj_count[MAX_K][N_STATES];
static int digit[N_STATES][5];  /* digits of each state in base‑3 */

/* fill digit table */
static void init_digit_table(void) {
    for (int s = 0; s < N_STATES; ++s) {
        int v = s;
        for (int col = 4; col >= 0; --col) {
            digit[s][col] = v % 3;
            v /= 3;
        }
    }
}

/* test if two states s,t are compatible for a given K:
   all four subblocks must give the same value, and that value
   must equal K.  Returns true if compatible. */
static bool compatible_pair(int s, int t, int K) {
    /* first subblock (columns 0 & 1) */
    int f0 = subblock_f(digit[s][0], digit[s][1],
                        digit[t][0], digit[t][1]);
    if (f0 != K) return false;
    for (int c = 1; c < 4; ++c) {
        int f = subblock_f(digit[s][c], digit[s][c+1],
                           digit[t][c], digit[t][c+1]);
        if (f != K) return false;
    }
    return true;
}

/* build adjacency lists for all K */
static void build_adj(void) {
    for (int K = 0; K < MAX_K; ++K) {
        for (int s = 0; s < N_STATES; ++s) {
            uint8_t cnt = 0;
            for (int t = 0; t < N_STATES; ++t) {
                if (compatible_pair(s, t, K)) {
                    adj[K][s][cnt++] = (uint8_t)t;
                }
            }
            adj_count[K][s] = cnt;
        }
    }
    built = true;
}

/* public function */
seq_t a(idx_t n)
{
    if (!built) {
        init_digit_table();
        build_adj();
    }

    seq_t total = 0;
    /* process each possible K separately */
    for (int K = 0; K < MAX_K; ++K) {
        /* dynamic programming over rows */
        static seq_t cur[N_STATES], next[N_STATES];
        /* initial: sequences of length 1 row (no transitions yet) */
        for (int s = 0; s < N_STATES; ++s)
            cur[s] = 1;
        /* perform n transitions (n steps) */
        for (idx_t step = 0; step < n; ++step) {
            for (int s = 0; s < N_STATES; ++s)
                next[s] = 0;          /* zero next array */

            for (int s = 0; s < N_STATES; ++s) {
                seq_t val = cur[s];
                if (val == 0) continue;
                uint8_t cnt = adj_count[K][s];
                for (uint8_t idx = 0; idx < cnt; ++idx) {
                    int t = adj[K][s][idx];
                    next[t] += val;
                }
            }
            /* swap cur and next */
            for (int s = 0; s < N_STATES; ++s)
                cur[s] = next[s];
        }
        /* sum over all ending states */
        seq_t sum_k = 0;
        for (int s = 0; s < N_STATES; ++s)
            sum_k += cur[s];
        total += sum_k;
    }
    return total;
}
