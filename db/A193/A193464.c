/*
 * A193464  Row sums of triangle A076731.
 * https://oeis.org/A193464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^n sum_{j=0}^k (-1)^j * binomial(k,j) * (n-j)! /
 *            (n-k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value a(n) is the sum of the n‑th row of triangle A076731. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* static storage for memoisation */
static idx_t max_computed = 0;          /* highest n for which a(n) is known */
static seq_t *prev_row = NULL;          /* B(n-1,0..n-1)  */
static size_t prev_cap = 0;
static seq_t *curr_row = NULL;          /* B(n,0..n)   */
static size_t curr_cap = 0;
static seq_t *a_values = NULL;          /* a(0..n)   */
static size_t a_cap = 0;

/* ensure that *arr has capacity at least 'need' elements */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = need;
        *arr = seq_realloc(*arr, *cap, new_cap);
        *cap = new_cap;
    }
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; a(0) is unused but return 0 */
        return (seq_t)0;

    /* if we already know a(n) return it */
    if (n <= max_computed)
        return a_values[n];

    /* first invocation */
    if (max_computed == 0) {
        /* initialise storage for the zero‑row (B(0,0)) */
        ensure_capacity(&prev_row, &prev_cap, 1);
        prev_row[0] = (seq_t)1;     /* B(0,0) = 1 */

        /* initialise arrays for the a‑values */
        ensure_capacity(&a_values, &a_cap, 1);
        a_values[0] = (seq_t)0;      /* a(0) is unused */

        max_computed = 0;
    }

    /* grow a_values if necessary */
    ensure_capacity(&a_values, &a_cap, (size_t)n + 1);

    /* compute rows from max_computed+1 up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        size_t need = (size_t)m + 1;          /* rows of length m+1 */

        ensure_capacity(&curr_row, &curr_cap, need);
        ensure_capacity(&prev_row, &prev_cap, (size_t)m);  /* prev has size m */

        /* B(m,0) = 1 */
        curr_row[0] = (seq_t)1;
        seq_t sum = (seq_t)0;

        for (idx_t k = 1; k <= m; ++k) {
            /* B(m,k) = B(m,k-1)*(m-k+1) - B(m-1,k-1) */
            seq_t term = curr_row[k - 1] * (seq_t)(m - k + 1) - prev_row[k - 1];
            curr_row[k] = term;
            sum = sum + term;
        }

        a_values[m] = sum;

        /* swap curr_row and prev_row for next iteration */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
        prev_cap = curr_cap;
        curr_cap = need;
    }

    max_computed = n;
    return a_values[n];
}
