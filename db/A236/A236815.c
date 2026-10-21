/*
 * A236815  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A236815
    a(n) = number of (n+1)×5 arrays with entries 0,1,2 such that for every
    2×2 subblock the value max + lower‑median − min is constant throughout
    the array.  The constant is not fixed a priori; each array determines it.
    The function uses a transfer‑matrix DP: for each possible constant k
    (0 ≤ k ≤ 4) we precompute the adjacency graph of the 243 possible 5‑tuples
    of row entries, then count walks of length n in that graph.  The total
    over all constants gives a(n).  Complexity O(n). */

#include <stdint.h>
#include <stdbool.h>

#define NROWS 243              /* 3^5 possible rows */

/* -------------------------------------------------------------------- */
/*  Global tables – built once by init_data()                            */
/* -------------------------------------------------------------------- */
static uint8_t  row[NROWS][5];                     /* digits of each row  */
static uint16_t adj[5][NROWS][NROWS];             /* adjacency lists       */
static uint8_t  deg[5][NROWS];                    /* degree of each row    */
static bool init_done = false;

/* -------------------------------------------------------------------- */
/*  Compute f(a,b,c,d) = max + lower_median – min for the four entries.   */
/* -------------------------------------------------------------------- */
static int block_value(int r1, int r2)
{
    int common = -1;
    for (int col = 0; col < 4; ++col)
    {
        int a = row[r1][col];
        int b = row[r1][col + 1];
        int c = row[r2][col];
        int d = row[r2][col + 1];
        /* max */
        int maxv = a > b ? a : b;
        if (c > maxv) maxv = c;
        if (d > maxv) maxv = d;
        /* min */
        int minv = a < b ? a : b;
        if (c < minv) minv = c;
        if (d < minv) minv = d;
        /* lower median – second smallest of the four numbers */
        int vals[4] = { a, b, c, d };
        /* insertion sort of 4 elements */
        for (int i = 1; i < 4; ++i) {
            int key = vals[i];
            int j = i - 1;
            while (j >= 0 && vals[j] > key) {
                vals[j + 1] = vals[j];
                --j;
            }
            vals[j + 1] = key;
        }
        int lower = vals[1];
        int f = maxv + lower - minv;
        if (common == -1)
            common = f;
        else if (f != common)
            return -1;          /* not constant over the 4 blocks */
    }
    return common; /* 0 … 4 */
}

/* -------------------------------------------------------------------- */
/*  Build the adjacency tables once.                                    */
/* -------------------------------------------------------------------- */
static void init_data(void)
{
    if (init_done)
        return;
    init_done = true;

    /* Enumerate all 5‑tuples over {0,1,2} */
    for (int r = 0; r < NROWS; ++r) {
        int x = r;
        for (int c = 4; c >= 0; --c) {
            row[r][c] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* For each pair of rows determine the constant f (if any) and
       fill adjacency lists for every f. */
    for (int i = 0; i < NROWS; ++i) {
        for (int j = i; j < NROWS; ++j) {
            int f = block_value(i, j);
            if (f >= 0) {
                adj[f][i][deg[f][i]++] = (uint16_t)j;
                if (i != j)
                    adj[f][j][deg[f][j]++] = (uint16_t)i;
            }
        }
    }
}

/* -------------------------------------------------------------------- */
/*  Public sequence function                                            */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_data();

    static seq_t dp[NROWS], dp_next[NROWS];
    seq_t total = 0;

    /* For each possible constant f 0…4 */
    for (int f = 0; f <= 4; ++f) {
        /* Initialize dp for the first row of the array */
        for (int r = 0; r < NROWS; ++r)
            dp[r] = 1;

        /* Perform n transitions (n = number of 2×2 blocks vertically) */
        for (idx_t step = 0; step < n; ++step) {
            /* Clear next dp */
            for (int r = 0; r < NROWS; ++r)
                dp_next[r] = 0;

            /* Propagate counts along all allowed edges */
            for (int u = 0; u < NROWS; ++u) {
                seq_t val = dp[u];
                if (val == 0)
                    continue;
                int d = deg[f][u];
                for (int k = 0; k < d; ++k) {
                    int v = adj[f][u][k];
                    dp_next[v] += val;
                }
            }
            /* Swap dp and dp_next */
            for (int r = 0; r < NROWS; ++r)
                dp[r] = dp_next[r];
        }

        /* Sum all arrays ending in any row */
        seq_t sum = 0;
        for (int r = 0; r < NROWS; ++r)
            sum += dp[r];
        total += sum;
    }
    return total;
}
