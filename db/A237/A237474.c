/*
 * A237474  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the lower
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays with entries 0..2 such that *
 *            every 2×2 subblock has the same value “maximum + lower median”.
 *            * The implementation counts walks of length n-1 in a 81‑state *
 *            automaton; a(1)=81 and * a(n) = Σ_{states} walks of length n‑1
 *            ending in that state
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define STATES 81            /* 9 possible rows × 9 possible rows */

static inline seq_t block_value(int a, int b, int c, int d)
{
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    if (d > max) max = d;

    int v[4] = {a, b, c, d};
    /* insertion sort of four elements */
    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 4; ++j)
            if (v[i] > v[j]) {
                int t = v[i];
                v[i] = v[j];
                v[j] = t;
            }
    int lower = v[1];
    return (seq_t)(max + lower);
}

/* adjacency data for the 81 states */
static seq_t state_val[STATES];
static int neighbors[STATES][STATES];
static unsigned char neigh_cnt[STATES];
static int graph_built = 0;

static void build_graph(void)
{
    /* pre–compute block values for all ordered pairs of rows */
    for (int p = 0; p < 9; ++p) {
        int pa = p / 3, pb = p % 3;
        for (int q = 0; q < 9; ++q) {
            int qa = q / 3, qb = q % 3;
            int idx = p * 9 + q;
            state_val[idx] = block_value(pa, pb, qa, qb);
        }
    }

    /* build adjacency lists: (p,q) → (q,r) if values are equal */
    for (int p = 0; p < 9; ++p) {
        for (int q = 0; q < 9; ++q) {
            int i = p * 9 + q;
            seq_t val = state_val[i];
            unsigned char cnt = 0;
            for (int r = 0; r < 9; ++r) {
                int j = q * 9 + r;
                if (state_val[j] == val) {
                    neighbors[i][cnt++] = j;
                }
            }
            neigh_cnt[i] = cnt;
        }
    }
    graph_built = 1;
}

/* memoisation cache */
static seq_t *memo = NULL;   /* memo[n] = a(n); 1‑based */
static idx_t  memo_n = 0;    /* highest n stored in memo */

/* DP arrays for walk lengths */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

seq_t a(idx_t n)
{
    if (!graph_built) {
        build_graph();
        /* initialise memo for n = 1 */
        memo = (seq_t *)malloc(sizeof(seq_t) * 2);   /* indices 0 and 1 */
        if (!memo) return (seq_t)0;
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)81;        /* a(1) = 81 */
        memo_n = 1;
        /* initialize dp_cur for step 1 (one state per row pair) */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)1;
        memset(dp_next, 0, sizeof(dp_next));
    }

    if (n == 0) return (seq_t)0;   /* undefined below offset 1 */

    if (n > memo_n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;
        memo = tmp;

        /* compute successive terms */
        for (idx_t step = memo_n + 1; step <= n; ++step) {
            memset(dp_next, 0, sizeof(dp_next));
            for (int i = 0; i < STATES; ++i) {
                seq_t val = dp_cur[i];
                if (val == (seq_t)0) continue;
                unsigned char cnt = neigh_cnt[i];
                int *nbr = neighbors[i];
                for (unsigned char k = 0; k < cnt; ++k)
                    dp_next[nbr[k]] += val;
            }
            memcpy(dp_cur, dp_next, sizeof(dp_cur));

            /* sum over all ending states */
            seq_t sum = (seq_t)0;
            for (int i = 0; i < STATES; ++i) sum += dp_cur[i];
            memo[step] = sum;
        }
        memo_n = n;
    }
    return memo[n];
}
