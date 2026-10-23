/*
 * A143081  A symmetrical triangle of coefficients based on A001147:
 *          a(n)=(2*n-1)*a(n-1); t(n,m)=a(n)^2/((2*n - 1)*a(m)*a(n - m)).
 * https://oeis.org/A143081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients based on A001147:
   Let d(k) = product_{i=1..k} (2*i-1)   (d(0)=1, d(1)=1, d(2)=3, …)
   Define t(n,m) (with 1 ≤ m ≤ n) by
          t(n,m) = d(n)^2 / ((2n-1) * d(m) * d(n-m))
   The sequence a(n) is the list of these triangle entries read
   row‑major with offset 1 (a(1) = -1).  Row sums are: {-1,2,5,36,…}.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 8:1 static table of double factorials d(k) = (2k-1)!!, d(0)=1.  */
static seq_t *dbl_fact = NULL;   /* index from 0 to current capacity */
static size_t dbl_fact_cap = 0;   /* last valid index in dbl_fact, 0 ⇔ only d(0) */

/* 1: Ensure d(k) is available for all 0 ≤ k ≤ max_n. */
static void ensure_dbl_fact(idx_t max_n)
{
    if ((idx_t)dbl_fact_cap >= max_n)
        return;                      /* already enough */

    size_t new_cap = (size_t)max_n + 1;          /* indices 0 … max_n inclusive */
    seq_t *new_tbl = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_tbl) abort();

    /* base case */
    new_tbl[0] = (seq_t)1;

    /* copy old data that exist */
    if (dbl_fact) {
        for (size_t i = 1; i <= dbl_fact_cap; ++i)   /* start at 1, skip 0 */
            new_tbl[i] = dbl_fact[i];
    }

    /* compute new values up to max_n */
    for (size_t i = dbl_fact_cap + 1; i <= (size_t)max_n; ++i) {
        new_tbl[i] = new_tbl[i-1] * (seq_t)(2*i - 1);
    }

    if (dbl_fact) free(dbl_fact);
    dbl_fact = new_tbl;
    dbl_fact_cap = new_cap - 1;          /* last valid index */
}

/* 2: Determine row r and position m (1‑based) from flattened index n. */
static void get_row_col(idx_t n, idx_t *r_out, idx_t *m_out)
{
    /* find the smallest r with r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cum = 1;                     /* terms up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev_terms = (r-1)*r/2;      /* terms before row r */
    idx_t m = n - prev_terms;          /* position in row, 1..r */
    *r_out = r;
    *m_out = m;
}

/* 3: Main function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 1)                  /* special first term */
        return (seq_t)(-1);

    /* Locate the row r and column m of the n‑th element of the triangle. */
    idx_t r, m;
    get_row_col(n, &r, &m);

    /* Ensure we have the double factorials up to r. */
    ensure_dbl_fact(r);

    /* Compute t(r,1) = d(r)^2 / ( (2r-1)*(2r-3)*d(r-2? ) ) simplified. */
    seq_t t;
    if (r == 2)                 /* t(2,1) = 1 by definition */
        t = (seq_t)1;
    else
        t = dbl_fact[r] / ((seq_t)(2*r - 1) * (seq_t)(2*r - 3));

    /* Walk across the row to reach column m. */
    for (idx_t k = 1; k < m; ++k) {
        seq_t num = (seq_t)(2*r - 2*k - 1);
        seq_t den = (seq_t)(2*k - 1);
        t = (t * num) / den;                 /* exact division */
    }

    return t;
}
