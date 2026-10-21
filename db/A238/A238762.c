/*
 * A238762  Triangle read by rows, generalized ballot numbers, 0<=k<=n.
 * https://oeis.org/A238762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A238762 (generalized ballot triangle)
 *
 *  T(0,0) = 1
 *  T(p,q) = 0                     if p < 0 or p > q
 *  T(p,q) = T(p-2,q) + (q mod 2)·T(p-1,q-1) + T(p,q-2)
 *
 *  The sequence a(n) is the triangle read row‑wise: a(0)=T(0,0),
 *  a(1)=T(0,1), a(2)=T(1,1), a(3)=T(0,2), a(4)=T(1,2), a(5)=T(2,2), …
 *
 *  The implementation builds rows of the triangle lazily and keeps
 *  them in a static cache so that repeated calls with arbitrary indices
 *  are answered in constant time after the required rows have been
 *  computed.  All arithmetic uses the supplied integer types seq_t
 *  and idx_t.
 */

#include <stdlib.h>

/* Triangle storage: each row i (0‑based) is an array of i+1 seq_t values.
 * triangle[i][k] == T(k,i).  The pointers themselves are stored in this
 * dynamic array; rows are allocated only when needed. */
static seq_t **triangle      = NULL;   /* pointer array of rows */
static size_t stored_rows    = 0;      /* number of rows currently stored */
static size_t allocated_rows = 0;      /* capacity of the triangle pointer array */

/* Allocate a zeroed row of length len and return a seq_t *.
 * For the arbitrary‑precision build, calloc returns void *; we explicitly
 * cast to seq_t * to satisfy C++ type checking. */
static seq_t *alloc_row(size_t len)
{
    return (seq_t *)calloc(len, sizeof(seq_t));
}

/* Ensure the triangle array has room for at least new_cap rows.
 * The realloc result is cast to seq_t ** to be C++ compliant. */
static void ensure_triangle_capacity(size_t new_cap)
{
    if (allocated_rows >= new_cap)
        return;
    size_t cap = allocated_rows ? allocated_rows : 1;
    while (cap < new_cap)
        cap <<= 1;
    seq_t **tmp = (seq_t **)realloc(triangle, cap * sizeof(seq_t *));
    triangle = tmp;
    allocated_rows = cap;
}

/* Compute a(n) as the row‑wise flattening of the triangle. */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n.
     * The first element of row r is at position r(r+1)/2.
     * We iterate until that position exceeds n. */
    idx_t r = 0;
    idx_t start = 0;
    while (start + (r + 1) <= n) {
        start += (r + 1);
        ++r;
    }
    /* Now start == r(r+1)/2 and r is the desired row. */
    idx_t k = n - start;        /* column within the row */

    /* Initialise the cache with the first row if it is empty. */
    if (stored_rows == 0) {
        ensure_triangle_capacity(1);
        triangle[0] = alloc_row(1);
        triangle[0][0] = (seq_t)1;    /* T(0,0) = 1 */
        stored_rows = 1;
    }

    /* Build rows up to r if not already present. */
    if (stored_rows <= r) {
        ensure_triangle_capacity(r + 1);

        for (idx_t i = stored_rows; i <= r; ++i) {
            size_t len = (size_t)i + 1;
            seq_t *row = alloc_row(len);

            for (idx_t j = 0; j <= i; ++j) {
                /* Apply the recurrence:
                 *
                 * T(j,i) = T(j-2,i) + (i mod 2)·T(j-1,i-1) + T(j,i-2)
                 *
                 * Terms that are out of the valid range are considered zero. */
                seq_t term1 = 0, term2 = 0, term3 = 0;

                if (j >= 2)
                    term1 = row[j - 2];            /* same row, earlier column */

                if ((i & 1) && j >= 1 && i >= 1)
                    term2 = triangle[i - 1][j - 1];/* previous row */

                if (i >= 2 && j <= i - 2)
                    term3 = triangle[i - 2][j];    /* two rows up, same column */

                /* Explicitly set T(0,0)=1; the recurrence would set 0+0+0. */
                if (i == 0 && j == 0)
                    row[j] = (seq_t)1;
                else
                    row[j] = term1 + term2 + term3;
            }
            triangle[i] = row;
        }
        stored_rows = r + 1;
    }

    /* Return the cached value. */
    return triangle[r][k];
}
