/*
 * A140993  Triangle G(n, k) read by rows, for 1 <= k <= n, where G(n, n) =
 *          G(n+1, 1) = 1, G(n+2, 2) = 2, G(n+3, m) = G(n+1, m-1) + G(n+1,
 *          m-2) + G(n+2, m-1) for n >= 1 and m = 3..(n+2).
 * https://oeis.org/A140993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A140993 – the triangle G(n,k) read by rows
   G(1,1) = 1
   G(n,1) = G(n,n) = 1            for n ≥ 1
   G(n,2) = 2                     for n ≥ 3
   G(n,k) = G(n-2,k-1) + G(n-2,k-2) + G(n-1,k-1)   for n ≥ 4, 3 ≤ k ≤ n-1
   The linear index n is obtained by listing the rows from left to right.
   a(n) = G(r,c) where r is the unique integer with r(r+1)/2 ≥ n and
   c = n – (r-1)r/2.  */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of the triangle */
static seq_t **triangle = NULL;   /* triangle[i] points to row i, 1‑based; element 0 unused */
static size_t maxRow = 0;

/* Ensure that rows 1..upto are computed and cached */
static void ensure_rows(size_t upto)
{
    if (upto <= maxRow)
        return;

    /* Allocate or reallocate the row pointer array */
    if (triangle == NULL) {
        triangle = (seq_t **)calloc(upto + 1, sizeof(seq_t *));
        if (!triangle) abort();
    } else {
        triangle = (seq_t **)realloc(triangle, (upto + 1) * sizeof(seq_t *));
        if (!triangle) abort();
        /* New rows are not initialized yet */
    }

    /* Build each new row incrementally using the recurrence */
    for (size_t i = maxRow + 1; i <= upto; ++i) {
        triangle[i] = (seq_t *)calloc(i + 1, sizeof(seq_t)); /* indices 1..i are used */
        if (!triangle[i]) abort();

        triangle[i][1] = (seq_t)1;
        triangle[i][i] = (seq_t)1;
        if (i >= 3)
            triangle[i][2] = (seq_t)2;

        if (i >= 4) {
            for (size_t k = 3; k <= i - 1; ++k) {
                seq_t val = triangle[i - 2][k - 1];
                val += triangle[i - 2][k - 2];
                val += triangle[i - 1][k - 1];
                triangle[i][k] = val;
            }
        }
    }

    maxRow = upto;
}

/* Return the a(n) term of A140993 */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; safe return */
        return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    size_t r = 1;
    while (((seq_t)r * ((seq_t)r + 1) / 2) < (seq_t)n)
        ++r;

    /* Column index c = n − (r−1)r/2 */
    seq_t prevTri = ((seq_t)(r - 1) * ((seq_t)(r - 1) + 1) / 2);
    idx_t c = n - (idx_t)prevTri;   /* casting safe because c ≤ r */

    /* Build necessary rows and fetch result */
    ensure_rows(r);
    return triangle[r][(size_t)c];
}
