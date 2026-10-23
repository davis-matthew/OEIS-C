/*
 * A026374  Triangular array T read by rows: T(n,0) = T(n,n) = 1 for all n >=
 *          0, T(n,k) = T(n-1,k-1) + T(n-1,k) for odd n and 1< = k <= n-1,
 *          T(n,k) = T(n-1,k-1) + T(n-1,k) + T(n-2,k-1) for even n and 1 <= k
 *          <= n-1.
 * https://oeis.org/A026374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the unique integer with r*(r+1)/2 <= n
 *            < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. T is defined by: T(n,0) =
 *            T(n,n) = 1 if n is odd and 1 <= k <= n-1: T(n,k) = T(n-1,k-1) +
 *            T(n-1,k) if n is even and 1 <= k <= n-1: T(n,k) = T(n-1,k-1) +
 *            T(n-1,k) + T(n-2,k-1)
 * Range:     u8 n <= 48, u16 n <= 142, u32 n <= 478, u64 n <= 1738,
 *            u128 n <= 6607, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for the flattened triangle and row start indices. */
static seq_t *triangle = NULL;        /* flattened triangle entries */
static size_t tri_len   = 0;          /* number of elements stored */
static size_t tri_cap   = 0;          /* allocated capacity of 'triangle'   */

static size_t *row_start = NULL;     /* start index of each row in 'triangle' */
static size_t row_start_len = 0;     /* number of rows stored */
static size_t row_start_cap = 0;     /* allocated capacity of 'row_start'   */

/* Ensure that triangle has capacity at least 'needed'. */
static void ensure_tri_capacity(size_t needed)
{
    if (needed > tri_cap) {
        size_t new_cap = tri_cap ? tri_cap * 2 : 128;
        if (new_cap < needed) new_cap = needed;
        triangle = (seq_t*)realloc(triangle, new_cap * sizeof(seq_t));
        tri_cap   = new_cap;
    }
}

/* Ensure that row_start has capacity at least 'needed'. */
static void ensure_row_start_capacity(size_t needed)
{
    if (needed > row_start_cap) {
        size_t new_cap = row_start_cap ? row_start_cap * 2 : 32;
        if (new_cap < needed) new_cap = needed;
        row_start = (size_t*)realloc(row_start, new_cap * sizeof(size_t));
        row_start_cap = new_cap;
    }
}

/* Append a new row (index r) to the flattened triangle. */
static void append_row(size_t r)
{
    size_t len = r + 1;                     /* number of elements in the row */

    /* Reserve space for the row's start index. */
    ensure_row_start_capacity(row_start_len + 1);
    row_start[row_start_len++] = tri_len;   /* start index of this new row */

    /* Ensure enough room for the new elements. */
    ensure_tri_capacity(tri_len + len);

    /* Compute each element in the row. */
    for (size_t k = 0; k < len; ++k) {
        seq_t val;
        if (k == 0 || k == len - 1) {       /* boundaries */
            val = (seq_t)1;
        } else if (r % 2 == 1) {            /* odd row: two-term sum */
            size_t idx_prev = row_start[r - 1];
            val = triangle[idx_prev + k - 1] + triangle[idx_prev + k];
        } else {                            /* even row: three-term sum */
            size_t idx_prev     = row_start[r - 1];
            size_t idx_prevprev = row_start[r - 2];
            val = triangle[idx_prev + k - 1] + triangle[idx_prev + k]
                + triangle[idx_prevprev + k - 1];
        }
        triangle[tri_len++] = val;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have computed enough of the triangle to cover index n. */
    while (tri_len <= (size_t)n) {
        size_t r = row_start_len;    /* the next row index to compute */
        append_row(r);
    }
    return triangle[(size_t)n];
}
