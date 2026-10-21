/*
 * A208696  Number of nX6 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A208696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A208696: number of n×6 binary arrays avoiding
 *  horizontal patterns 0 0 1 and 1 0 0, and vertical patterns 0 1 0 and 1 0 1.
 *  The function computes the count by dynamic programming over
 *  two‑row states.  If n=1 the answer is the number of allowed rows;
 *  if n=2 it is the square of that, etc.  The table of adjacency lists
 *  is constructed once when the function is first called.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const int COLS = 6;                /* width of each row        */
static const int MAX_PAT = 1 << COLS;     /* 64 possible 6‑bit rows   */

static int allowed_cnt = 0;               /* number of allowed rows   */
static uint8_t *allowed = NULL;           /* array of allowed masks   */
static int *idx_of = NULL;                /* mapping pattern → index  */

static int state_cnt = 0;                 /* number of two‑row states */
static int **adj = NULL;                  /* adjacency lists         */
static int *deg = NULL;                   /* out‑degree of each state */

static bool init_done = false;

/* --------------------------------------------------------------------- */
/*  Initialise data structures – run once on first call                 */
/* --------------------------------------------------------------------- */
static void init_data(void)
{
    int i, k, c, j, a, b, r;
    int M;                 /* number of allowed rows      */
    int s, dest;

    /* Allocate tables that are sized by the number of possible patterns */
    idx_of = (int *)calloc(MAX_PAT, sizeof(int));
    allowed = (uint8_t *)calloc(MAX_PAT, sizeof(uint8_t));

    /* Find allowed row patterns – horizontal 000? and 001? forbidden */
    for (i = 0; i < MAX_PAT; ++i) {
        bool good = true;
        for (k = 0; k + 2 < COLS; ++k) {
            int b0 = (i >> k)     & 1;
            int b1 = (i >> (k+1)) & 1;
            int b2 = (i >> (k+2)) & 1;
            if ((b0==0 && b1==0 && b2==1) || (b0==1 && b1==0 && b2==0)) {
                good = false; break;
            }
        }
        if (good) {
            idx_of[i] = allowed_cnt;
            allowed[allowed_cnt++] = (uint8_t)i;
        } else {
            idx_of[i] = -1;
        }
    }
    M = allowed_cnt;        /* number of allowed rows              */

    /* Two‑row state space size */
    state_cnt = M * M;
    adj   = (int **)calloc(state_cnt, sizeof(int *));
    deg   = (int  *)calloc(state_cnt, sizeof(int ));

    /* Build adjacency lists */
    for (s = 0; s < state_cnt; ++s) {
        a = s / M;                 /* top row index of the state */
        b = s % M;                 /* middle row index            */

        /* First count how many successors are possible */
        int d = 0;
        for (c = 0; c < M; ++c) {
            r = allowed[c];
            bool ok = true;
            for (j = 0; j < COLS; ++j) {
                int p = (allowed[a] >> j) & 1;
                int q = (allowed[b] >> j) & 1;
                int t = (r      >> j) & 1;
                /* vertical patterns 0 1 0 or 1 0 1 forbidden */
                if ((p==0 && q==1 && t==0) || (p==1 && q==0 && t==1)) {
                    ok = false; break;
                }
            }
            if (ok) ++d;
        }

        deg[s] = d;
        if (d > 0) {
            adj[s] = (int *)malloc(d * sizeof(int));
            int *p = adj[s];
            for (c = 0; c < M; ++c) {
                r = allowed[c];
                bool ok = true;
                for (j = 0; j < COLS; ++j) {
                    int p_bit = (allowed[a] >> j) & 1;
                    int q_bit = (allowed[b] >> j) & 1;
                    int r_bit = (r          >> j) & 1;
                    if ((p_bit==0 && q_bit==1 && r_bit==0) ||
                        (p_bit==1 && q_bit==0 && r_bit==1)) {
                        ok = false; break;
                    }
                }
                if (ok) *p++ = b * M + c;   /* new state (b,c) */
            }
        } else {
            adj[s] = NULL;            /* theoretical, never used      */
        }
    }
    init_done = true;
}

/* --------------------------------------------------------------------- */
/*  Sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init_data();

    int M = allowed_cnt;          /* number of allowed rows       */
    if (n == 1) return (seq_t)M;                   /* single row */
    if (n == 2) return (seq_t)M * (seq_t)M;          /* two rows */

    /* DP arrays for current and next row count */
    seq_t *dp   = (seq_t *)calloc(state_cnt, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(state_cnt, sizeof(seq_t));

    /* Base case: all pairs of allowed rows possible for n=2 */
    for (int a = 0; a < M; ++a)
        for (int b = 0; b < M; ++b)
            dp[a * M + b] = 1;

    /* Transition for rows 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < state_cnt; ++s) next[s] = 0;
        for (int s = 0; s < state_cnt; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            for (int k = 0; k < deg[s]; ++k) {
                next[ adj[s][k] ] += val;
            }
        }
        /* swap dp and next */
        seq_t *tmp = dp; dp = next; next = tmp;
    }

    /* Sum over all end states – this is the total number of arrays */
    seq_t result = 0;
    for (int s = 0; s < state_cnt; ++s) result += dp[s];

    free(dp);
    free(next);
    return result;
}
