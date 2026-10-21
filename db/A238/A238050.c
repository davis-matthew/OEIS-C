/*
 * A238050  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A238050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 such that for
 *            every 2×2 subblock T the statistic
 *            s(T)=max+min−upperMedian−lowerMedian differs from that of its
 *            horizontal and vertical neighbors by exactly one
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   abs_int, compute_stat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic‑programming approach over row pairs.
   The array rows are encoded as base‑4 numbers 0..63 (three digits).  For a
   pair of consecutive rows (a,b) we pre‑compute the statistics of the two
   2×2 blocks that use those rows and enforce the horizontal constraint.
   Transitions between pairs enforce both horizontal and vertical
   neighbour constraints.  The DP counts all valid fillings of (n+1) rows. */
 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>   /* for uint64_t */
#include "../utils/abs_int.h"
#include "../utils/compute_stat.h"

/* helper used by the precomputation (no external name clashes) */

/* -------------------------------------------------------------------- */
/* Pre‑computation tables for all row pairs (64×64)                    */
/* -------------------------------------------------------------------- */
static int V;                 /* number of horizontally valid pairs */
static int *pair_first;       /* first row of a valid pair           */
static int *pair_second;      /* second row of a valid pair          */
static int *pair_f0;          /* statistic of the left 2×2 block     */
static int *pair_f1;          /* statistic of the right 2×2 block    */
static int **next_list;       /* adjacency lists for state graph      */
static int *next_cnt;         /* outgoing degree of each state        */

static void precompute(void)
{
    /* tables for all 64×64 row pairs */
    static int f0pair[64][64];
    static int f1pair[64][64];
    int pair_idx[64][64];
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            pair_idx[i][j] = -1;

    /* compute statistics for every row pair */
    for (int a = 0; a < 64; ++a) {
        int a0 = a >> 4, a1 = (a >> 2) & 3, a2 = a & 3;
        for (int b = 0; b < 64; ++b) {
            int b0 = b >> 4, b1 = (b >> 2) & 3, b2 = b & 3;
            f0pair[a][b] = compute_stat(a0, a1, b0, b1);
            f1pair[a][b] = compute_stat(a1, a2, b1, b2);
        }
    }

    /* enumerate valid horizontal pairs */
    V = 0;
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b)
            if (abs_int(f0pair[a][b] - f1pair[a][b]) == 1)
                pair_idx[a][b] = V++;

    pair_first  = (int*)malloc(V * sizeof(int));
    pair_second = (int*)malloc(V * sizeof(int));
    pair_f0     = (int*)malloc(V * sizeof(int));
    pair_f1     = (int*)malloc(V * sizeof(int));

    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b) {
            int idx = pair_idx[a][b];
            if (idx >= 0) {
                pair_first[idx]  = a;
                pair_second[idx] = b;
                pair_f0[idx]     = f0pair[a][b];
                pair_f1[idx]     = f1pair[a][b];
            }
        }

    /* count outgoing edges for each valid pair */
    next_cnt = (int*)calloc(V, sizeof(int));
    for (int i = 0; i < V; ++i) {
        int a = pair_first[i];
        int b = pair_second[i];
        for (int t = 0; t < 64; ++t) {
            int f0new = f0pair[b][t];
            int f1new = f1pair[b][t];
            if (abs_int(f0new - f1new) != 1) continue;
            if (abs_int(pair_f0[i] - f0new) != 1) continue;
            if (abs_int(pair_f1[i] - f1new) != 1) continue;
            int j = pair_idx[b][t];
            if (j < 0) continue;
            ++next_cnt[i];
        }
    }

    next_list = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; ++i)
        next_list[i] = (int*)malloc(next_cnt[i] * sizeof(int));

    /* fill adjacency lists */
    int *pos = (int*)calloc(V, sizeof(int));
    for (int i = 0; i < V; ++i) {
        int a = pair_first[i];
        int b = pair_second[i];
        for (int t = 0; t < 64; ++t) {
            int f0new = f0pair[b][t];
            int f1new = f1pair[b][t];
            if (abs_int(f0new - f1new) != 1) continue;
            if (abs_int(pair_f0[i] - f0new) != 1) continue;
            if (abs_int(pair_f1[i] - f1new) != 1) continue;
            int j = pair_idx[b][t];
            if (j < 0) continue;
            next_list[i][pos[i]++] = j;
        }
    }
    free(pos);
}

/* -------------------------------------------------------------------- */
/* Public function required by the framework                           */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset 1 */

    /* initialise static data once */
    static int initialized = 0;
    if (!initialized) {
        precompute();
        initialized = 1;
    }

    /* DP arrays */
    seq_t *dp      = (seq_t*)calloc(V, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(V, sizeof(seq_t));

    /* base: all valid first two rows contribute one arrangement each */
    for (int i = 0; i < V; ++i)
        dp[i] = (seq_t)1;

    /* number of additional rows to add beyond the first two */
    int loops = (n > 1) ? (int)(n - 1) : 0;

    for (int step = 0; step < loops; ++step) {
        for (int i = 0; i < V; ++i) {
            seq_t val = dp[i];
            if (val == (seq_t)0) continue;
            const int *list = next_list[i];
            int cnt = next_cnt[i];
            for (int k = 0; k < cnt; ++k) {
                int j = list[k];
                dp_next[j] += val;
            }
        }
        /* swap and clear next array */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
        memset(dp_next, 0, sizeof(seq_t) * V);
    }

    /* sum all ways for the final state */
    seq_t total = (seq_t)0;
    for (int i = 0; i < V; ++i)
        total += dp[i];

    free(dp);
    free(dp_next);
    return total;
}
