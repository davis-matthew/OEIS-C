/*
 * A209126  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209127; see the Formula section.
 * https://oeis.org/A209126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle T(n,k):
   T(1,0) = 1
   T(2,0) = 2, T(2,1) = 1
   T(n,k) = T(n-1,k) + T(n-1,k-1) + T(n-2,k-2)      (n ≥ 3, 0 ≤ k ≤ n-1)
   T(n,k) = 0 if k < 0 or k > n-1
   The sequence A209126 is the triangle read row‑by‑row in standard
   order.  a(n) = T(r,k) where r and k are chosen so that
   r*(r+1)/2 ≥ n and k = n - r*(r-1)/2 - 1.  */

#include <stdlib.h>

static seq_t **triangle = NULL;   /* array of row pointers, indexed by row number */
static idx_t  rows_generated = 0; /* highest row computed so far (≥0) */
static size_t rows_capacity  = 0; /* current capacity of the triangle array */

/* Ensure that all rows up to and including 'n' are computed and stored
   in the global 'triangle' array.  */
static void
ensure_rows(idx_t n)
{
    /* Increase the size of the triangle array if necessary.  */
    if (n >= rows_capacity)
    {
        size_t new_cap = rows_capacity ? rows_capacity * 2 : 1;
        while (new_cap <= n) new_cap *= 2;          /* guarantee new_cap > n      */
        seq_t **tmp = (seq_t **)realloc(triangle,
                                       new_cap * sizeof(seq_t *));
        if (!tmp) exit(EXIT_FAILURE);              /* allocation failed           */
        for (size_t i = rows_capacity; i < new_cap; ++i)
            tmp[i] = NULL;                         /* initialise new slots        */
        triangle = tmp;
        rows_capacity = new_cap;
    }

    /* Generate any rows that are still missing.  */
    for (idx_t r = rows_generated + 1; r <= n; ++r)
    {
        seq_t *row = (seq_t *)malloc((size_t)r * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        if (r == 1)
        {
            row[0] = (seq_t)1;
        }
        else if (r == 2)
        {
            row[0] = (seq_t)2;
            row[1] = (seq_t)1;
        }
        else
        {
            seq_t *prev1 = triangle[r - 1];
            seq_t *prev2 = triangle[r - 2];
            for (idx_t k = 0; k < r; ++k)
            {
                seq_t val = 0;
                if (k < r - 1)            /* T(n-1,k)   */
                    val += prev1[k];
                if (k >= 1)                /* T(n-1,k-1) */
                    val += prev1[k - 1];
                if (k >= 2)                /* T(n-2,k-2) */
                    val += prev2[k - 2];
                row[k] = val;
            }
        }
        triangle[r] = row;
    }
    rows_generated = n;
}

/* Public function required by the test harness.  */
seq_t
a(idx_t n)
{
    /* Locate the row r that contains the n-th entry (1‑based).  */
    idx_t r = 1;
    while ((seq_t)r * ((seq_t)r + 1) / 2 < n) ++r;

    /* Column index inside that row (0‑based).  */
    idx_t prev_sum = r * (r - 1) / 2;
    idx_t k = n - prev_sum - 1;

    /* Ensure the requested row has been computed.  */
    ensure_rows(r);

    return triangle[r][k];
}
