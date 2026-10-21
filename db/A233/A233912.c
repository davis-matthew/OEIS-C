/*
 * A233912  Number of (n+1)X(3+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 30, and no
 *          two adjacent values equal.
 * https://oeis.org/A233912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   ok_2x2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A233912
   Method: dynamic programming over all admissible 1×4 rows.
   A row is a tuple (a,b,c,d) with 0≤a,b,c,d≤7, no equal adjacent cells.
   Transitions are allowed when rows are vertically compatible
   (no equal column entries) and every 2×2 subblock satisfies
   (a‑b)²+(c‑d)²+(a‑c)²+(b‑d)² = 30.
   The number of (n+1)×4 tables is the sum over all ending rows
   after n+1 rows have been processed.                                     */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ok_2x2.h"

/* Row representation */
typedef struct { uint8_t v[4]; } Row;

/* Constants */
static const int ROWS_COUNT = 2744;  /* 8*7*7*7 */

/* Static adjacency tables */
static int  *adj_cnt = NULL;   /* number of neighbors for each state */
static int **adj_list = NULL;  /* adjacency lists indexed by state */

/* Build all admissible rows */
static void build_rows(Row *rows)
{
    int idx = 0;
    for (uint8_t a = 0; a < 8; ++a)
        for (uint8_t b = 0; b < 8; ++b)
            if (b != a)
                for (uint8_t c = 0; c < 8; ++c)
                    if (c != b)
                        for (uint8_t d = 0; d < 8; ++d)
                            if (d != c)
                            {
                                rows[idx].v[0] = a;
                                rows[idx].v[1] = b;
                                rows[idx].v[2] = c;
                                rows[idx].v[3] = d;
                                ++idx;
                            }
}

/* Precompute transition lists */
static void init_transitions(void)
{
    Row rows[ROWS_COUNT];
    build_rows(rows);

    adj_cnt   = (int *)malloc(ROWS_COUNT * sizeof(int));
    adj_list  = (int **)malloc(ROWS_COUNT * sizeof(int *));
    for (int i = 0; i < ROWS_COUNT; ++i)
    {
        int cap = 8;
        int *tmp = (int *)malloc(cap * sizeof(int));
        int cnt = 0;
        for (int j = 0; j < ROWS_COUNT; ++j)
        {
            /* vertical adjacency */
            bool ok = true;
            for (int k = 0; k < 4 && ok; ++k)
                if (rows[i].v[k] == rows[j].v[k]) ok = false;

            /* 2×2 constraints for the three column pairs */
            if (ok)
            {
                for (int col = 0; col < 3 && ok; ++col)
                    if (!ok_2x2(rows[i].v[col], rows[i].v[col + 1],
                                rows[j].v[col], rows[j].v[col + 1]))
                        ok = false;
            }

            if (ok)
            {
                if (cnt == cap)
                {
                    cap <<= 1;
                    tmp = (int *)realloc(tmp, cap * sizeof(int));
                }
                tmp[cnt++] = j;
            }
        }
        adj_cnt[i]  = cnt;
        adj_list[i] = tmp;
    }
}

/* Computes a(n) for n >= 1.  External linkage. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        init_transitions();
        initialized = true;
    }

    /* number of rows in the array: L = n + 1 */
    size_t L = (size_t)n + 1;
    if (L < 2) return 0;           /* no valid arrays for L<2 */

    /* DP arrays */
#ifdef __cplusplus
    seq_t *dp      = new seq_t[ROWS_COUNT];
    seq_t *dp_next = new seq_t[ROWS_COUNT];
#else
    seq_t *dp      = (seq_t *)malloc(ROWS_COUNT * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc(ROWS_COUNT * sizeof(seq_t));
#endif

    /* initial state: any row is allowed */
    for (int i = 0; i < ROWS_COUNT; ++i) dp[i] = 0;
    for (int i = 0; i < ROWS_COUNT; ++i) dp[i] = 1;

    /* iterate through the remaining rows */
    for (size_t step = 1; step < L; ++step)
    {
        for (int i = 0; i < ROWS_COUNT; ++i) dp_next[i] = 0;

        for (int i = 0; i < ROWS_COUNT; ++i)
        {
            seq_t val = dp[i];
            if (val == 0) continue;
            int *neibs = adj_list[i];
            int cnt    = adj_cnt[i];
            for (int k = 0; k < cnt; ++k)
                dp_next[neibs[k]] += val;
        }
        seq_t *tmp = dp; dp = dp_next; dp_next = tmp;
    }

    /* sum of all ending rows gives the required count */
    seq_t total = 0;
    for (int i = 0; i < ROWS_COUNT; ++i) total += dp[i];

#ifdef __cplusplus
    delete[] dp;
    delete[] dp_next;
#else
    free(dp);
    free(dp_next);
#endif
    return total;
}
