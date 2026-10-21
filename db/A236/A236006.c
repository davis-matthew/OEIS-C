/*
 * A236006  Number of (n+1)X(4+1) 0..3 arrays with the minimum plus the
 *          maximum equal to the lower median plus the upper median in every
 *          2X2 subblock.
 * https://oeis.org/A236006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computing sequence A236006:
   a(n) = number of (n+1)×5 arrays with entries in {0,1,2,3}
          such that in every 2×2 subblock the min plus the max
          equals the lower median plus the upper median.  The
          value is obtained by a dynamic–programming count over
          the 5‑column row patterns. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define ROWS 1024           /* 4^5 */
#define MAX_DEG 256         /* safe upper bound for degree */
#define MAX_N 10001          /* maximum n we expect to handle */

static bool prop_table[4][4][4][4];
static bool adjacency_built = false;
static int adjacency[ROWS][MAX_DEG];
static int deg[ROWS];
static uint8_t row_val[ROWS][5];

static seq_t dp_cur[ROWS];
static seq_t dp_next[ROWS];
static seq_t ans_cache[MAX_N+2];
static int cached_n = -1;   /* highest n for which ans_cache is filled */

/* Build property table and adjacency lists once. */
static void build_adjacency(void)
{
    if (adjacency_built) return;
    adjacency_built = true;

    /* property table: true if 2x2 block (x,y,u,v) satisfies a+d=b+c */
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            for (int u = 0; u < 4; ++u)
                for (int v = 0; v < 4; ++v) {
                    int t[4] = {x, y, u, v};
                    /* find a<=b<=c<=d */
                    int a = t[0], b = t[1], c = t[2], d = t[3];
                    /* simple sorting */
                    if (a > b) { int tmp=a; a=b; b=tmp; }
                    if (c > d) { int tmp=c; c=d; d=tmp; }
                    if (a > c) { int tmp=a; a=c; c=tmp; }
                    if (b > d) { int tmp=b; b=d; d=tmp; }
                    if (a > b) { int tmp=a; a=b; b=tmp; }
                    prop_table[x][y][u][v] = (a + d == b + c);
                }

    /* decode each row index into its 5 digits */
    for (int r = 0; r < ROWS; ++r) {
        int idx = r;
        for (int j = 4; j >= 0; --j) {
            row_val[r][j] = (uint8_t)(idx & 3);
            idx >>= 2;
        }
    }

    /* build adjacency lists */
    for (int a = 0; a < ROWS; ++a) {
        deg[a] = 0;
        uint8_t *va = row_val[a];
        for (int b = 0; b < ROWS; ++b) {
            uint8_t *vb = row_val[b];
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                if (!prop_table[va[j]][va[j+1]][vb[j]][vb[j+1]]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                adjacency[a][deg[a]++] = b;
            }
        }
    }
}

/* Ensure dp_cur and dp_next are initialized for n=0. */
static void init_dp(void)
{
    for (int i = 0; i < ROWS; ++i) {
        dp_cur[i] = (seq_t)1;
    }
    /* first cached answer (n = 0) is the number of arrays of size 1 row */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < ROWS; ++i) sum += dp_cur[i];
    ans_cache[0] = sum;
    cached_n = 0;
}

/* Compute all answers up to required n. */
static void compute_upto(idx_t n)
{
    if (cached_n < 0) init_dp();
    for (int k = cached_n + 1; k <= (int)n; ++k) {
        /* dp_next[b] = sum over a of dp_cur[a] if a is adjacent to b */
        for (int b = 0; b < ROWS; ++b) dp_next[b] = (seq_t)0;
        for (int a = 0; a < ROWS; ++a) {
            seq_t va = dp_cur[a];
            if (va == 0) continue;
            int dg = deg[a];
            int *nbr = adjacency[a];
            for (int t = 0; t < dg; ++t) {
                dp_next[nbr[t]] += va;
            }
        }
        /* prepare for next iteration */
        for (int i = 0; i < ROWS; ++i) dp_cur[i] = dp_next[i];

        /* compute total for this n (=k) */
        seq_t total = (seq_t)0;
        for (int i = 0; i < ROWS; ++i) total += dp_cur[i];
        ans_cache[k] = total;
    }
    cached_n = (int)n;
}

/* Public interface */
seq_t a(idx_t n)
{
    /* a(1) corresponds to 2 rows */
    if (n < 1) return (seq_t)0;   /* undefined below offset */
    if (!adjacency_built) build_adjacency();
    compute_upto(n);
    return ans_cache[n];
}
