/*
 * A204420  Triangle T(n,k) giving number of degree-2n permutations which
 *          decompose into exactly k cycles of even length, k=0..n.
 * https://oeis.org/A204420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(m,k) where m = floor((√(8n+1)-1)/2), k = n – m(m+1)/2.
 *            The triangle T is defined by T(0,0) = 1, T(n,0) = 0 (n>0) T(n,k)
 *            = (2n-1) * ( T(n-1,k-1) + 2*(n-1)*T(n-1,k) )
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 28, u64 n <= 66, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed for size_t and abort() */
#include <stddef.h>
#include <stdlib.h>

/* Global storage for the triangle */
static seq_t **triangle = NULL;   /* rows of the triangle T   */
static size_t rows_computed = 0; /* number of rows already built */

/* ------------------------------------------------------------ */
/* Compute the integer m such that m*(m+1)/2 ≤ n < (m+1)(m+2)/2.   */
static idx_t triangular_root(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* Find an upper bound that is too large */
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* Binary search for the largest high with the property. */
    while (high > low) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------ */
/* Ensure that rows 0 … max_row inclusive are computed.   */
static void ensure_triangle_upto(idx_t max_row)
{
    if (rows_computed > max_row) return;

    /* Allocate or resize the array of row pointers. */
    seq_t **new_tri = (seq_t **)realloc(triangle,
                                        (max_row + 1) * sizeof(seq_t*));
    if (!new_tri) abort();  /* unrecoverable */
    triangle = new_tri;

    for (idx_t n = rows_computed; n <= max_row; ++n) {
        /* Allocate a row of length n+1.  Use a cast so the code compiles
           in both C and C++ (the build may provide an arbitrary‑precision
           class for seq_t). */
        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!row) abort();
        triangle[n] = row;

        if (n == 0) {            /* T(0,0) = 1 */
            row[0] = 1;
        } else {
            row[0] = 0;          /* T(n,0) = 0 for n>0 */
            seq_t *prev = triangle[n - 1];
            for (idx_t k = 1; k <= n; ++k) {
                seq_t term1 = prev[k - 1];
                seq_t term2 = 0;
                if (k <= n - 1)   /* prev[k] exists only up to n-1 */
                    term2 = (seq_t)(2 * (n - 1)) * prev[k];
                seq_t sum = term1 + term2;
                row[k] = (seq_t)(2 * n - 1) * sum;
            }
        }
    }
    rows_computed = max_row + 1;
}

/* ------------------------------------------------------------ */
/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Determine the parameters m,k for the requested position. */
    idx_t m = triangular_root(n);
    idx_t tri = m * (m + 1) / 2;
    idx_t k = n - tri;          /* 0 ≤ k ≤ m */

    ensure_triangle_upto(m);
    return triangle[m][k];
}
