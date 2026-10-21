/*
 * A210793  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210794; see the Formula section.
 * https://oeis.org/A210793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle T(n,k) (0‑based indices):
   Base rows:
      T(0,0) = 1
      T(1,0) = 2,  T(1,1) = 1
   For n ≥ 2:
      T(n,k) = T(n-1,k-1) + 3·T(n-2,k) + 2·T(n-2,k-1) + T(n-2,k-2)
   (Terms outside the range 0 ≤ k ≤ n are treated as 0.)
   The sequence a(n) is the flat list of the triangle taken row by row.
   a(n) corresponds to the entry T(r-1,k) where r = ⌈(-1+√(1+8n))/2⌉
   (1‑based row number) and k = n – (r-1)·r/2 – 1 (0‑based column). */

#include <stdlib.h>
#include <math.h>
#include "../utils/seq_alloc_new_array.h"

static seq_t **triangle = NULL;     /* pointers to rows */
static size_t tri_cap  = 0;         /* capacity of triangle array */
static size_t tri_len  = 0;         /* number of rows stored (0‑based) */

/* Ensure that rows 0 … r are computed and stored. */
static void ensure_rows(idx_t r)
{
    while (tri_len <= (size_t)r) {
        /* grow the array of row pointers if needed */
        if (tri_len >= tri_cap) {
            size_t new_cap = tri_cap ? tri_cap * 2 : 4;
            seq_t **tmp = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
            if (!tmp) abort();
            triangle = tmp;
            tri_cap  = new_cap;
        }

        /* allocate space for the new row (length = tri_len + 1) */
        size_t row_len = tri_len + 1;
        seq_t *row = (seq_t *)seq_alloc_new_array(row_len);
        if (!row) abort();

        if (tri_len == 0) {                   /* row 0: length 1 */
            row[0] = (seq_t)1;
        } else if (tri_len == 1) {            /* row 1: length 2 */
            row[0] = (seq_t)2;
            row[1] = (seq_t)1;
        } else {                              /* rows n ≥ 2 */
            seq_t *prev1 = triangle[tri_len - 1];   /* row n-1, length n */
            seq_t *prev2 = triangle[tri_len - 2];   /* row n-2, length n-1 */
            size_t prev1_len = tri_len;             /* n */
            size_t prev2_len = tri_len - 1;         /* n-1 */

            for (size_t k = 0; k <= tri_len; ++k) {
                seq_t val = 0;

                /* T(n-1,k-1) */
                if (k >= 1)
                    val += prev1[k - 1];

                /* 3·T(n-2,k)  */
                if (k < prev2_len)
                    val += (seq_t)3 * prev2[k];

                /* 2·T(n-2,k-1) */
                if (k >= 1 && k - 1 < prev2_len)
                    val += (seq_t)2 * prev2[k - 1];

                /* T(n-2,k-2) */
                if (k >= 2 && k - 2 < prev2_len)
                    val += prev2[k - 2];

                row[k] = val;
            }
        }
        triangle[tri_len++] = row;
    }
}

/* Return the n‑th term of A210793 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    /* Compute the 1‑based row number r such that r*(r+1)/2 ≥ n. */
    long double disc = sqrt((long double)1 + 8.0L * (long double)n);
    idx_t r = (idx_t)((disc - 1.0L) / 2.0L);

    /* Adjust upward if necessary. */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* Row index in zero‑based terms (row length = r). */
    idx_t row_index = r - 1;
    ensure_rows(row_index);

    /* Number of terms before this row. */
    unsigned __int128 prev_total = (unsigned __int128)row_index * (row_index + 1) / 2;

    /* Zero‑based column index within the row. */
    idx_t k = (idx_t)(n - prev_total - 1);

    return triangle[row_index][k];
}
