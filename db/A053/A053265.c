/*
 * A053265  Coefficients of the '5th-order' mock theta function F_1(q).
 * https://oeis.org/A053265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 167, u32 n <= 533, u64 n <= 1848,
 *            u128 n <= 6761, big any n
 * Helpers:   part_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = Σ_{m≥0, 2m(m+1)≤n} p_{odd≤2m+1}(n-2m(m+1))
 * where p_{odd≤x}(k) is the number of partitions of k into odd parts ≤ x.
 * The code pre‑computes all needed partition counts and then sums the
 * required terms.  It works for arbitrary‑precision seq_t and is fast
 * enough for n up to about 10000 (and larger if requested).
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/part_index.h"

/* seq_t and idx_t are supplied by the harness; do not define them. */

#define PRECOMP_MAX_N 10000   /* initial pre‑compilation size */

/* Global state for memoisation                                         */
static idx_t precomp_N = 0;          /* maximum n that has been computed     */
static idx_t precomp_M = 0;          /* maximum m for that N                 */
static seq_t *seq_data  = NULL;      /* a(n) values                          */
static seq_t *part_data = NULL;      /* partition counts: part_data[m*(N+1)+k] */
static idx_t part_rows = 0;          /* number of rows in part_data (precomp_M+1) */
static idx_t part_cols = 0;          /* number of columns (precomp_N+1) */

/* Recompute all tables up to the new maximum n.  Assumes new_N >= 1. */
static void recompute_tables(idx_t new_N)
{
    /* largest m with 2*m*(m+1) <= new_N */
    idx_t new_M = (idx_t)((sqrt(1.0 + 2.0 * (double)new_N) - 1.0) / 2.0);

    /* allocate sequence array */
    seq_data = (seq_t *)realloc(seq_data, sizeof(seq_t) * (new_N + 1));
    /* allocate partition table */
    part_data = (seq_t *)realloc(part_data,
                                 sizeof(seq_t) * (new_M + 1) * (new_N + 1));

    part_rows = new_M + 1;
    part_cols = new_N + 1;

    /* temporary DP array for partitions with odd parts */
    seq_t *dp = (seq_t *)calloc((size_t)(new_N + 1), sizeof(seq_t));
    dp[0] = (seq_t)1;                     /* empty partition */

    /* compute partition counts row by row (each row adds one more odd part) */
    for (idx_t m = 0; m <= new_M; ++m) {
        idx_t part = 2 * m + 1;           /* new odd part to include */

        for (idx_t j = part; j <= new_N; ++j) {
            dp[j] += dp[j - part];
        }

        /* copy current row into part_data */
        for (idx_t k = 0; k <= new_N; ++k) {
            part_data[part_index(m, k, new_N)] = dp[k];
        }
    }
    free(dp);

    /* compute a(n) for all n up to new_N using the precomputed partition table */
    for (idx_t n = 0; n <= new_N; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t m = 0; m <= new_M; ++m) {
            idx_t offset = 2 * m * (m + 1);
            if (offset > n) break;
            sum += part_data[part_index(m, n - offset, new_N)];
        }
        seq_data[n] = sum;
    }

    precomp_N = new_N;
    precomp_M = new_M;
}

/* public accessor ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* first call or n larger than previously computed range */
    if (precomp_N == 0) {
        idx_t init_N = PRECOMP_MAX_N;
        if (n > init_N) init_N = n;
        recompute_tables(init_N);
    } else if (n > precomp_N) {
        recompute_tables(n);
    }

    return seq_data[(size_t)n];
}
