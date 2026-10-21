/*
 * A236881  Number of (n+1)X(5+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A236881: number of (n+1)×6 0..2 arrays whose
   maximum + upper median + lower median of every 2×2 subblock is
   the same constant.  The value is obtained by dynamic programming
   over the rows of the array. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROWS 729          /* 3^6 possible rows of 6 entries in {0,1,2} */
#define MAX_C    7            /* constants 0..6 */

static int row_vals[MAX_ROWS][6];          /* base‑3 representation of each row */

/* adjacency lists: adj[c][u] holds the neighbors of row u for constant c */
struct Adj {
    int *neigh;        /* array of neighbor row indices */
    int cnt;           /* number of neighbors */
};
static struct Adj adj[MAX_C][MAX_ROWS];

/* memoised DP arrays */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* -------------------------------------------------------------------- */
/* Build all adjacency lists once.  Each row is represented as an array
   of 6 digits in {0,1,2}.  For a given pair of rows (u,v) we compute
   the value s = sum of the 4 entries in the 2×2 block minus its
   minimum.  If s is the same for all 5 column positions and
   0≤s≤6, we insert v into the adjacency list of u for constant s. */
static void build_adjacency(void)
{
    int u, v, ci, j, a, b, c, d, sum, mn, s, s0;
    int cnt[MAX_C][MAX_ROWS] = {0};

    /* generate all rows */
    for (u = 0; u < MAX_ROWS; ++u) {
        int x = u;
        for (j = 5; j >= 0; --j) {
            row_vals[u][j] = x % 3;
            x /= 3;
        }
    }

    /* first pass: count neighbours per row per constant */
    for (u = 0; u < MAX_ROWS; ++u) {
        for (v = 0; v < MAX_ROWS; ++v) {
            bool ok = true;
            s0 = -1;
            for (j = 0; j < 5; ++j) {
                a = row_vals[u][j];
                b = row_vals[u][j + 1];
                c = row_vals[v][j];
                d = row_vals[v][j + 1];
                sum = a + b + c + d;
                mn = a;
                if (b < mn) mn = b;
                if (c < mn) mn = c;
                if (d < mn) mn = d;
                s = sum - mn;           /* max + lower median + upper median */
                if (j == 0) s0 = s;
                else if (s != s0) { ok = false; break; }
            }
            if (ok && 0 <= s0 && s0 <= 6) {
                ci = s0;            /* constant index directly 0..6 */
                ++cnt[ci][u];
            }
        }
    }

    /* allocate memory for adjacency lists and fill them */
    for (ci = 0; ci < MAX_C; ++ci) {
        for (u = 0; u < MAX_ROWS; ++u) {
            adj[ci][u].cnt = cnt[ci][u];
            if (cnt[ci][u] > 0)
                adj[ci][u].neigh = (int *)malloc(cnt[ci][u] * sizeof(int));
            else
                adj[ci][u].neigh = NULL;
        }
    }

    /* second pass: fill adjacency lists */
    int there[MAX_C][MAX_ROWS] = {0};   /* current insertion position per row */
    for (u = 0; u < MAX_ROWS; ++u) {
        for (v = 0; v < MAX_ROWS; ++v) {
            bool ok = true;
            s0 = -1;
            for (j = 0; j < 5; ++j) {
                a = row_vals[u][j];
                b = row_vals[u][j + 1];
                c = row_vals[v][j];
                d = row_vals[v][j + 1];
                sum = a + b + c + d;
                mn = a;
                if (b < mn) mn = b;
                if (c < mn) mn = c;
                if (d < mn) mn = d;
                s = sum - mn;
                if (j == 0) s0 = s;
                else if (s != s0) { ok = false; break; }
            }
            if (ok && 0 <= s0 && s0 <= 6) {
                ci = s0;
                int pos = there[ci][u]++;
                adj[ci][u].neigh[pos] = v;
            }
        }
    }
}

/* -------------------------------------------------------------- */
/* Main sequence function.  The first row can be arbitrary; each
   additional row must satisfy the constant condition with the
   preceding row.  The number of valid arrays is the number of walks
   of length n over the adjacency graph. */
seq_t a(idx_t n)
{
    if (dp_cur == NULL) {           /* lazy allocation of DP arrays */
        dp_cur  = (seq_t *)calloc(MAX_ROWS, sizeof(seq_t));
        dp_next = (seq_t *)calloc(MAX_ROWS, sizeof(seq_t));
        if (dp_cur == NULL || dp_next == NULL)
            return (seq_t)0;        /* out of memory – unlikely in tests */
    }

    /* build adjacency once */
    static bool built = false;
    if (!built) { build_adjacency(); built = true; }

    seq_t result = (seq_t)0;
    int ci, u, v;                              /* indices */

    for (ci = 0; ci < MAX_C; ++ci) {
        /* if this constant has no edges, skip */
        bool has_edge = false;
        for (u = 0; u < MAX_ROWS; ++u)
            if (adj[ci][u].cnt > 0) { has_edge = true; break; }
        if (!has_edge) continue;

        /* initialise dp_cur with 1 for each starting row */
        for (u = 0; u < MAX_ROWS; ++u) dp_cur[u] = (seq_t)1;

        /* propagate n times (n steps transforms n+1 rows) */
        for (idx_t step = 0; step < n; ++step) {
            for (u = 0; u < MAX_ROWS; ++u) dp_next[u] = (seq_t)0;
            for (u = 0; u < MAX_ROWS; ++u) {
                seq_t val = dp_cur[u];
                if (val == (seq_t)0) continue;
                int neigh_cnt = adj[ci][u].cnt;
                int *neigh = adj[ci][u].neigh;
                for (int k = 0; k < neigh_cnt; ++k) {
                    v = neigh[k];
                    dp_next[v] += val;
                }
            }
            /* swap dp_cur and dp_next */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }

        /* sum remaining counts */
        seq_t sum = (seq_t)0;
        for (u = 0; u < MAX_ROWS; ++u) sum += dp_cur[u];
        result += sum;
    }
    return result;
}
