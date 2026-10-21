/*
 * A236887  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A236887: Number of (n+1)×3 arrays of digits 0..2 such that
   for every 2×2 subblock the value MAX+UMID-MMID-MIN is the same
   constant (i.e. all subblocks share the same f).  Computed by
   dynamic programming over the 27 possible rows.  The constant f
   can take values 0,1,2,3 or 4. */

#include <stdint.h>
#include <stdbool.h>

/* sort 4 integers into ascending order – used by f‑value computation */
static void sort4(int *x)
{
    for (int i = 1; i < 4; ++i) {
        int key = x[i];
        int j = i - 1;
        while (j >= 0 && x[j] > key) {
            x[j + 1] = x[j];
            --j;
        }
        x[j + 1] = key;
    }
}

/* compute f = max + upper median – lower median – min for four values */
static int compute_f(int a0, int a1, int a2, int a3)
{
    int v[4] = {a0, a1, a2, a3};
    sort4(v);
    return v[3] + v[2] - v[1] - v[0];   /* 0 <= f <= 4 */
}

/* adjacency for each possible constant f ∈ {0,1,2,3,4} */
static bool ready = false;
static int neighcnt[5][27];
static int neigh[5][27][27];

/* build the graph once */
static void build_graph(void)
{
    for (int f = 0; f < 5; ++f)
        for (int i = 0; i < 27; ++i)
            neighcnt[f][i] = 0;

    /* rows coded in base‑3: r = d0*9 + d1*3 + d2   (d0 top, d1 middle, d2 bottom) */
    for (int r = 0; r < 27; ++r) {
        int dr0 = r / 9;
        int dr1 = (r / 3) % 3;
        int dr2 = r % 3;
        for (int s = 0; s < 27; ++s) {
            int ds0 = s / 9;
            int ds1 = (s / 3) % 3;
            int ds2 = s % 3;

            int f1 = compute_f(dr0, dr1, ds0, ds1);
            int f2 = compute_f(dr1, dr2, ds1, ds2);
            if (f1 == f2) {
                int f = f1;               /* 0..4 */
                int idx = neighcnt[f][r]++;
                neigh[f][r][idx] = s;
            }
        }
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) build_graph();

    seq_t total = 0;
    seq_t dp[27];
    seq_t ndp[27];

    for (int f = 0; f < 5; ++f) {
        /* start with any first row */
        for (int i = 0; i < 27; ++i) dp[i] = 1;

        /* perform n steps (n transitions) */
        for (idx_t step = 1; step <= n; ++step) {
            for (int i = 0; i < 27; ++i) ndp[i] = 0;

            for (int r = 0; r < 27; ++r) {
                seq_t val = dp[r];
                if (!val) continue;
                int cnt = neighcnt[f][r];
                for (int k = 0; k < cnt; ++k) {
                    int s = neigh[f][r][k];
                    ndp[s] += val;
                }
            }
            for (int i = 0; i < 27; ++i) dp[i] = ndp[i];
        }

        /* add all arrays with constant f */
        for (int i = 0; i < 27; ++i) total += dp[i];
    }
    return total;
}
