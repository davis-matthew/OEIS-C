/*
 * A237069  Number of (n+1)X(1+1) 0..3 arrays with the upper median of every
 *          2X2 subblock equal.
 * https://oeis.org/A237069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   sort4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A237069: Number of (n+1)×2 0..3 arrays with the upper median of every
    2×2 subblock equal.
    Count of arrays = Σ_{M=0..3} (number of arrays whose 2×2 blocks all
    share the same upper median M).  The computation is performed by
    dynamic programming over the 16 possible row states (x,y), 0≤x,y≤3. */
#include <stdlib.h>
#include "../utils/sort4.h"

/* adjacency[M][s][t] == 1 iff rows s=(x,y) and t=(u,v) satisfy that
   the upper median of the 2×2 block formed by them equals M.  States
   are numbered s = 4*x + y. */
static unsigned char adjacency[4][16][16] = {0};
static int adjacency_initialized = 0;

/* Initialize adjacency once. */
static void init_adjacency(void)
{
    int i, j, x, y, u, v, M;
    if (adjacency_initialized) return;
    for (M = 0; M <= 3; ++M)
        for (i = 0; i < 16; ++i) {
            x = i / 4;
            y = i % 4;
            for (j = 0; j < 16; ++j) {
                u = j / 4;
                v = j % 4;
                int arr[4] = {x, y, u, v};
                sort4(arr);
                adjacency[M][i][j] = (arr[2] == M) ? 1 : 0;
            }
        }
    adjacency_initialized = 1;
}

/* Count arrays for a fixed upper‑median value M and length n+1 rows. */
static seq_t count_for_M(idx_t n, int M)
{
    seq_t dp_cur[16], dp_next[16];
    int i, t, s;
    /* initial: exactly one array of length 1 ending at each state */
    for (s = 0; s < 16; ++s) dp_cur[s] = (seq_t)1;

    /* perform n transitions (from 1 row to n+1 rows) */
    for (i = 1; i <= (int)n; ++i) {
        for (t = 0; t < 16; ++t) dp_next[t] = (seq_t)0;
        for (s = 0; s < 16; ++s) {
            if (!dp_cur[s]) continue;
            for (t = 0; t < 16; ++t)
                if (adjacency[M][s][t])
                    dp_next[t] += dp_cur[s];
        }
        /* copy next to current for next iteration */
        for (t = 0; t < 16; ++t) dp_cur[t] = dp_next[t];
    }

    /* sum over all possible final states */
    seq_t total = (seq_t)0;
    for (t = 0; t < 16; ++t) total += dp_cur[t];
    return total;
}

/* Public sequence routine: a(n) for n ≥ 1, 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of range guard */
    init_adjacency();
    seq_t sum = (seq_t)0;
    int M;
    for (M = 0; M <= 3; ++M)
        sum += count_for_M(n, M);
    return sum;
}
