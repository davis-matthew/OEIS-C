/*
 * A232598  T(n,k) = Stirling2(n,k) * OrderedBell(k).
 * https://oeis.org/A232598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 62, u64 n <= 164,
 *            u128 n <= 453, big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A232598:  T(n,k) = Stirling2(n,k) * OrderedBell(k)
 *  a(n) is the T(p,k) value where
 *      p = ceil( (sqrt(8*n+1)-1)/2 )   // row index
 *      k = n - p*(p-1)/2               // column index (1 ≤ k ≤ p)
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/seq_mul.h"

/*--- Stirling numbers of the second kind ---------------------------------*/
static seq_t **S_rows   = NULL;   /* S_rows[n][k]  (0 ≤ k ≤ n) */
static idx_t  S_rows_cnt = 0;     /* number of rows already computed */

static void
ensure_stirling(idx_t max)
{
    if (max < S_rows_cnt) return;

    /* enlarge pointer array */
    S_rows = (seq_t ** )realloc(S_rows, sizeof(seq_t *) * (max + 1));

    /* allocate new rows and initialise */
    for (idx_t i = S_rows_cnt; i <= max; ++i) {
        S_rows[i] = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!S_rows[i]) exit(1);
        if (i == 0) S_rows[0][0] = (seq_t)1;
    }

    /* build rows from S_rows_cnt to max */
    for (idx_t n = S_rows_cnt; n <= max; ++n) {
        if (n == 0) continue;          /* base row already set */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t a = (k <= n - 1) ? S_rows[n - 1][k] : (seq_t)0;
            seq_t b = S_rows[n - 1][k - 1];
            S_rows[n][k] = a * (seq_t)k + b;   /* T(n,k) = k*T(n-1,k)+T(n-1,k-1) */
        }
    }

    S_rows_cnt = max + 1;
}

/*--- Ordered Bell numbers -------------------------------------------------*/
static seq_t *OB        = NULL;   /* OB[i]  (0 ≤ i) */
static idx_t  OB_len    = 0;      /* highest index computed + 1 */

static void
ensure_ordered_bell(idx_t max)
{
    if (max < OB_len) return;

    /* enlarge OB array */
    OB = (seq_t *)realloc(OB, sizeof(seq_t) * (max + 1));

    if (OB_len == 0) {                /* initialise OB[0] = 1 */
        OB[0] = (seq_t)1;
        OB_len = 1;
    }

    /* compute OB[n] for n = OB_len .. max */
    for (idx_t n = OB_len; n <= max; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < n; ++j) {
            seq_t bj  = binom(n, j);                  /* binomial(n,j) */
            sum = sum + seq_mul(bj, OB[j]);           /* bj * OB[j] */
        }
        OB[n] = sum;
    }

    OB_len = max + 1;
}

/*--- Main function for the sequence -------------------------------------*/
seq_t
a(idx_t n)          /* n ≥ 1 (offset 1) */
{
    if (n == 0) return (seq_t)0;   /* not expected for offset 1 */

    /* locate the triangular pair (p,k) corresponding to the index n */
    idx_t p = 1;
    while ((seq_t)p * (p + 1) / 2 < (seq_t)n) ++p;

    idx_t prev = (p - 1) * p / 2;
    idx_t k    = n - prev;                /* 1‑based column, 1 ≤ k ≤ p */

    /* make sure the needed data are present */
    ensure_stirling(p);
    ensure_ordered_bell(k);

    /* T(p,k) = S2(p,k) * OrderedBell(k) */
    return seq_mul(S_rows[p][k], OB[k]);
}
