/*
 * A207092  Number of 6Xn 0..1 arrays avoiding 0 0 0 horizontally and 0 0 1
 *          vertically.
 * https://oeis.org/A207092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 6 × n 0..1 arrays avoiding 0 0 0 horizontally and 0 0
 *            1 vertically
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*--------------------------------------------------------------------
 *  Static tables are built on first call.  No external libraries are
 *  required beyond the standard C library and the built‑in seq_t type.
 *--------------------------------------------------------------------*/
static bool initialized = false;
static int F;                     /* number of vertical‑allowed columns       */
static int cols[64];              /* masks of allowed columns                 */
static idx_t S;                   /* total number of states (F * F)           */
static int **nextList;            /* adjacency lists  [state] -> list of k   */
static int *nextCount;            /* number of entries in each list           */

static void build_tables(void)
{
    int i, j, k, r;
    /* 1. Enumerate all 6‑bit columns that avoid the vertical pattern 0 0 1 */
    F = 0;
    for (i = 0; i < 64; ++i) {
        bool ok = true;
        for (r = 0; r <= 3 && ok; ++r) {
            int b0 = (i >> r)     & 1;
            int b1 = (i >> (r+1)) & 1;
            int b2 = (i >> (r+2)) & 1;
            if (b0 == 0 && b1 == 0 && b2 == 1) ok = false;
        }
        if (ok) {
            cols[F++] = i;
        }
    }

    S = (idx_t)F * (idx_t)F;

    /* 2. Allocate adjacency structures */
    nextList   = (int **)malloc((size_t)S * sizeof(int *));
    nextCount  = (int *)malloc((size_t)S * sizeof(int));
    if (!nextList || !nextCount) exit(1); /* failure to allocate */

    /* 3. For every pair (i,j) compute list of allowed next columns k  */
    int tmp[64];
    for (i = 0; i < F; ++i) {
        for (j = 0; j < F; ++j) {
            int state = i * F + j;
            int cnt = 0;
            for (k = 0; k < F; ++k) {
                bool ok = true;
                for (r = 0; r < 6 && ok; ++r) {
                    int b0 = (cols[i] >> r) & 1;
                    int b1 = (cols[j] >> r) & 1;
                    int b2 = (cols[k] >> r) & 1;
                    if (b0 == 0 && b1 == 0 && b2 == 0) ok = false;
                }
                if (ok) tmp[cnt++] = k;
            }
            nextCount[state] = cnt;
            int *list = (int *)malloc((size_t)cnt * sizeof(int));
            memcpy(list, tmp, (size_t)cnt * sizeof(int));
            nextList[state] = list;
        }
    }

    initialized = true;
}

/*--------------------------------------------------------------------
 *   Main counting routine.  It runs for arbitrary n >= 1.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) build_tables();

    /* Small n can be answered directly */
    if (n == 1) return (seq_t)F;
    if (n == 2) {
        seq_t v = (seq_t)F;
        return v * v;
    }

    /* Allocate dynamic programming arrays */
    seq_t *dp_cur  = (seq_t *)calloc((size_t)S, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc((size_t)S, sizeof(seq_t));
    if (!dp_cur || !dp_next) exit(1);

    /* Initialise dp for column pairs (c1 , c2) */
    for (int i = 0; i < F; ++i)
        for (int j = 0; j < F; ++j)
            dp_cur[i * F + j] = 1;

    /* Run the DP for columns 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(dp_next, 0, (size_t)S * sizeof(seq_t));

        for (int state = 0; state < S; ++state) {
            seq_t val = dp_cur[state];
            if (val == 0) continue;
            int *list = nextList[state];
            int cnt   = nextCount[state];
            int j = state % F;            /* last column of current pair  */
            for (int t = 0; t < cnt; ++t) {
                int k = list[t];
                int nextState = j * F + k;
                dp_next[nextState] += val;
            }
        }
        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum over all states to obtain the final count */
    seq_t total = 0;
    for (int i = 0; i < S; ++i)
        total += dp_cur[i];

    return total;
}
