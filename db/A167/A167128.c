/*
 * A167128  Triangle T(m,n) read by rows: T(m,n) = Sum_{k=1..n} StirlingS2(m,
 *          n) * StirlingS2(m, k).
 * https://oeis.org/A167128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = S2(m,n) * sum_{k=1..n} S2(m,k), where m is the smallest
 *            integer with m*(m+1)/2 ≥ n. S2(m,k) denotes the Stirling numbers
 *            of the second kind
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 58, u64 n <= 126,
 *            u128 n <= 334, big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* static storage for Stirling numbers and running column sums */
static seq_t **S2_rows    = NULL;   /* S2_rows[m][k] : S2(m,k), 0 ≤ k ≤ m */
static seq_t **prefix_rows = NULL; /* prefix_rows[m][k] : Σ_{i=1..k} S2(m,i) */
static idx_t   max_row     = 0;      /* largest row that has been computed   */
static size_t  alloc_cap   = 0;      /* current capacity of the pointer arrays */

/* Ensure that rows 1..row are available and computed */
static void ensure_rows(idx_t row)
{
    if (row <= max_row) return;

    /* grow the pointer arrays if necessary */
    if ((size_t)(row + 1) > alloc_cap)
    {
        size_t new_cap = (size_t)(row + 1);
        seq_t **tmp;

        tmp = (seq_t **)realloc(S2_rows,        new_cap * sizeof(seq_t *));
        S2_rows = tmp;
        tmp = (seq_t **)realloc(prefix_rows,    new_cap * sizeof(seq_t *));
        prefix_rows = tmp;
        alloc_cap = new_cap;
    }

    /* compute all missing rows */
    for (idx_t r = max_row + 1; r <= row; ++r)
    {
        /* allocate storage for this row */
        S2_rows[r]     = seq_alloc_array((size_t)(r + 1));
        prefix_rows[r] = seq_alloc_array((size_t)(r + 1));

        /* index 0 is unused, set to zero */
        S2_rows[r][0]     = (seq_t)0;
        prefix_rows[r][0] = (seq_t)0;

        if (r == 1)
        {
            /* base case */
            S2_rows[1][1]     = (seq_t)1;
            prefix_rows[1][1] = (seq_t)1;
        }
        else
        {
            seq_t *S2_prev = S2_rows[r - 1];
            seq_t *S2_cur  = S2_rows[r];
            seq_t pref_sum = (seq_t)0;

            for (idx_t k = 1; k <= r; ++k)
            {
                /* Compute S2(r,k) = S2(r-1,k-1) + k * S2(r-1,k) */
                seq_t term = S2_prev[k - 1];
                if (k <= r - 1) {
                    term += (seq_t)k * S2_prev[k];
                }
                S2_cur[k] = term;
                pref_sum += term;
                prefix_rows[r][k] = pref_sum;
            }
        }
    }
    max_row = row;
}

seq_t a(idx_t n)
{
    /* Find the row m that contains the n‑th element.
       The total number of elements up to row m is m*(m+1)/2. */
    idx_t m = 1;
    idx_t cumulative = 1;
    while (cumulative < n)
    {
        ++m;
        cumulative += m;
    }

    /* Column index (1‑based) within this row */
    idx_t prev_cumulative = cumulative - m; /* (m-1)*m/2 */
    idx_t k = n - prev_cumulative;          /* 1 ≤ k ≤ m */

    /* Ensure Stirling numbers up to row m are available */
    ensure_rows(m);

    /* Compute T(m,k) = S2(m,k) * sum_{i=1..k} S2(m,i) */
    return S2_rows[m][k] * prefix_rows[m][k];
}
