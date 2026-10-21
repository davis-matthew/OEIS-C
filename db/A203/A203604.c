/*
 * A203604  Number of n X 2 0,1 arrays with adjacent row and column sums
 *          unequal.
 * https://oeis.org/A203604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A203604: Number of n×2 0/1 matrices with adjacent row sums unequal
 * and column sums unequal.
 *
 * Algorithm:
 * 1. Let M(n) be the total number of n×2 matrices with no equal adjacent
 *    row sums (ignoring column sums).  A DP over the three possible row
 *    sums {0,1,2} with weight 2 for the sum‑1 rows gives M(n) in linear
 *    time.
 *
 * 2. Let B(n) be the number of such matrices whose two column sums are
 *    equal.  For each row the difference “#ones in column 0 - #ones in column 1”
 *    changes by 0 (rows 00 or 11) or ±1 (rows 10 and 01).  We keep a DP over
 *    the current last row sum and this difference.  The coefficient of
 *    difference 0 after n rows is B(n).  The state space is
 *    3 × (2n+1) and the recurrence is linear.
 *
 * 3. a(n) = M(n) – B(n).  The values are stored in a static cache so
 *    that subsequent calls with any index are O(1).
 *
 * The routine works with any seq_t / idx_t supplied by the build
 * (built‑in unsigned integer types or arbitrary‑precision integer class).
 *
 * Note: we allocate enough space for n up to 10 000, which covers the
 * intended range of the test suite. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------  main function  ---------------- */
seq_t a(idx_t n)
{
    /* Never called with n == 0 in the series; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* static cache so that repeated calls are cheap. */
    static seq_t *cache     = NULL;   /* cache[1..MAXN] */
    static size_t cache_cap = 0;      /* cache_cap == MAXN+1 */
    static idx_t  top       = 0;      /* largest n already computed */

    /* constants for the DP */
    static const idx_t MAXN = 10000;          /* upper bound for n */
    static int   len      = 0;                /* 2*MAXN+1 */
    static int   offset   = 0;                /* MAXN */

    /* matrices for B(n) DP */
    static seq_t *cur   = NULL;  /* 3 * len */
    static seq_t *next  = NULL;  /* 3 * len */

    /* state for M(n) DP */
    static seq_t v0 = 0, v1 = 0, v2 = 0;      /* current step values */

    /* First initialisation */
    if (cache == NULL) {
        cache_cap = (size_t)MAXN + 1u;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        len   = 2 * MAXN + 1;
        offset = MAXN;

        cur  = (seq_t *)calloc(3 * len, sizeof(seq_t));
        next = (seq_t *)calloc(3 * len, sizeof(seq_t));
        if (!cache || !cur || !next) exit(EXIT_FAILURE);

        /* B(1)  – initialise the DP for step 1 */
        cur[0 * len + offset]           = (seq_t)1;   /* sum 0, d = 0 */
        cur[2 * len + offset]           = (seq_t)1;   /* sum 2, d = 0 */
        cur[1 * len + (offset + 1)]     = (seq_t)1;   /* sum 1, d = +1 */
        cur[1 * len + (offset - 1)]     = (seq_t)1;   /* sum 1, d = -1 */

        /* M(1) – initialise the DP for step 1 */
        v0 = (seq_t)1;   /* sum 0  -> 1 way */
        v1 = (seq_t)2;   /* sum 1  -> 2 orientations */
        v2 = (seq_t)1;   /* sum 2  -> 1 way */

        /* store a(1) = M1 - B1 = 4 - 2 = 2 */
        cache[1] = (v0 + v1 + v2) -
                   (cur[0 * len + offset] +
                    cur[1 * len + offset] +
                    cur[2 * len + offset]);

        top = 1;
    }

    /* compute all terms up to n (if not already done) */
    for (idx_t step = top + 1; step <= n; ++step) {
        /* ---------- M(n) DP ---------- */
        seq_t nv0 = v1 + v2;
        seq_t nv1 = (v0 + v2) * (seq_t)2;
        seq_t nv2 = v0 + v1;
        v0 = nv0; v1 = nv1; v2 = nv2;

        /* ---------- B(n) DP (move one step) ---------- */
        /* zero the 'next' array (size 3*len) */
        size_t total = 3 * (size_t)len;
        for (size_t i = 0; i < total; ++i) next[i] = 0;

        int d_low  = offset - (int)(step - 1);
        int d_high = offset + (int)(step - 1);

        /* transition from each previous state */
        for (int j = 0; j < 3; ++j) {
            seq_t *cst  = cur  + j * len;
            seq_t *nst0 = next + 0 * len;
            seq_t *nst1 = next + 1 * len;
            seq_t *nst2 = next + 2 * len;

            for (int d = d_low; d <= d_high; ++d) {
                seq_t val = cst[d];
                if (!val) continue;

                if (j != 0) nst0[d] += val;
                if (j != 2) nst2[d] += val;
                if (j != 1) {
                    if (d + 1 < len) nst1[d + 1] += val;
                    if (d - 1 >= 0) nst1[d - 1] += val;
                }
            }
        }

        /* swap cur and next for the next iteration */
        seq_t *tmp = cur; cur = next; next = tmp;

        /* compute B(step) = coefficient of d = 0 */
        seq_t B = cur[0 * len + offset] +
                  cur[1 * len + offset] +
                  cur[2 * len + offset];

        /* store a(step) = M(step) – B(step) */
        cache[step] = (v0 + v1 + v2) - B;
    }

    top = n;
    return cache[n];
}
