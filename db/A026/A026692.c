/*
 * A026692  Triangular array T read by rows: T(n,0)=T(n,n)=1 for n >= 0; for n
 *          >= 2 and 1<=k<=n-1, T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k) if k or
 *          n-k is of form 2h for h=1,2,...,[ n/4 ], else
 *          T(n,k)=T(n-1,k-1)+T(n-1,k).
 * https://oeis.org/A026692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A026692: Triangular array read by rows
   T(n,0)=T(n,n)=1 for n>=0;
   For n>=2,1<=k<=n-1
      if k or n-k is 2,4,6,…,2⌊n/4⌋
         T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k)
      else
         T(n,k)=T(n-1,k-1)+T(n-1,k)
   a(n) = the nth entry when the triangle is read row‑by‑row
   (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t **tri = NULL;          /* array of row pointers */
static size_t rows_used = 0;        /* number of rows actually built (0‑based) */

/* Build all rows up to and including 'row' (0‑based).  Returns true
   on success, false if a memory allocation fails. */
static bool build_rows(size_t row)
{
    if (row < rows_used)
        return true;           /* already built */

    /* Grow the pointer array if necessary */
    if (tri == NULL || row + 1 > rows_used) {
        size_t new_size = row + 1;
        seq_t **tmp = (seq_t **)realloc(tri, new_size * sizeof(seq_t *));
        if (!tmp)
            return false;      /* allocation failed; caller will ignore */
        tri = tmp;
    }

    /* Build missing rows */
    for (size_t n = rows_used; n <= row; ++n) {
        seq_t *row_ptr = (seq_t *)calloc(n + 1U, sizeof(seq_t));
        if (!row_ptr)
            return false;      /* allocation failed */

        row_ptr[0] = row_ptr[n] = (seq_t)1;   /* T(n,0)=T(n,n)=1 */

        if (n >= 2U) {
            size_t limit = n / 4U;            /* ⌊n/4⌋ */
            for (size_t k = 1U; k < n; ++k) {
                seq_t a = tri[n - 1U][k - 1U];
                seq_t b = tri[n - 1U][k];
                bool include = false;
                size_t h = k / 2U;
                if ((k & 1U) == 0U && h >= 1U && h <= limit)
                    include = true;
                h = (n - k) / 2U;
                if (((n - k) & 1U) == 0U && h >= 1U && h <= limit)
                    include = true;
                if (include)
                    row_ptr[k] = a + b + tri[n - 2U][k - 1U];
                else
                    row_ptr[k] = a + b;
            }
        } else if (n == 1U) {
            /* For n==1 the loop above is skipped; row_ptr[1] already set to 1 */
        }

        tri[n] = row_ptr;
    }

    rows_used = row + 1U;
    return true;
}

/* Find the row r (0-indexed) that contains index n (1‑based).
   It satisfies (r+1)(r+2)/2 >= n.  Uses binary search. */
static size_t row_of(idx_t n)
{
    size_t low = 0U;
    size_t high = 1U;
    /* Exponentially increase upper bound until it covers n */
    while ((seq_t)(high + 1) * (seq_t)(high + 2) / 2U < (seq_t)n)
        high *= 2U;
    /* Binary search in [low, high] */
    while (low < high) {
        size_t mid = low + (high - low) / 2U;
        if ((seq_t)(mid + 1) * (seq_t)(mid + 2) / 2U >= (seq_t)n)
            high = mid;
        else
            low = mid + 1U;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;

    /* Determine the row that contains the index */
    size_t r = row_of(n);

    /* Ensure rows up to r are built */
    if (!build_rows(r))
        return (seq_t)0;           /* In case of allocation failure */

    /* Compute position within the row (0‑based) */
    seq_t prev_terms = (seq_t)r * (seq_t)(r + 1U) / 2U;
    size_t k = (size_t)(n - prev_terms - (seq_t)1U);   /* 0 <= k <= r */

    return tri[r][k];
}
