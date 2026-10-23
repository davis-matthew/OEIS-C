/*
 * A157154  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) - m*k*(n-k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1 and m = 3, read by rows.
 * https://oeis.org/A157154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   Triangle T(n,k,3) defined by T(n,0)=T(n,n)=1, T(n,k) =
 *            (3*(n-k)+1)*T(n-1,k-1) + (3*k+1)*T(n-1,k) - 3*k*(n-k)*T(n-2,k-1)
 *            for n≥2, 1≤k≤n-1 The sequence a(n) is the triangle read
 *            row‑wise: a(0)=1, a(1)=1, a(2)=1, …
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 59, u64 n <= 158,
 *            u128 n <= 415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Public function that returns the nth term of OEIS A157154.
   The types idx_t and seq_t are supplied externally by the build system. */
seq_t a(idx_t n)
{
    /* Locate the row r and column k of the element with linear index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    /* Static cache for rows of the triangle.  Each row i contains i+1
       entries, accessible as triangle[i][j]. */
    static seq_t **triangle = NULL;
    static idx_t rows = 0;   /* number of rows currently stored              */

    /* Allocate or grow the row pointer array if necessary. */
    if (rows <= r) {
        seq_t **tmp = (seq_t **)realloc(triangle, (r + 1) * sizeof(seq_t *));
        if (!tmp) return (seq_t)0;   /* unlikely in test environment          */
        triangle = tmp;

        /* Build each missing row, starting from the first unbuilt one. */
        for (idx_t i = rows; i <= r; ++i) {
            seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            if (!row) return (seq_t)0;   /* allocation failure (unlikely)      */

            for (idx_t j = 0; j <= i; ++j) {
                if (j == 0 || j == i) {
                    row[j] = (seq_t)1;   /* boundary values */
                } else {
                    /* Recurrence terms – cast after multiplication
                       to enforce truncation to seq_t.           */
                    seq_t term1 = (seq_t)
                        ((seq_t)(3 * (i - j) + 1) * (seq_t)triangle[i - 1][j - 1]);
                    seq_t term2 = (seq_t)
                        ((seq_t)(3 * j + 1) * (seq_t)triangle[i - 1][j]);
                    seq_t term3 = (seq_t)
                        ((seq_t)(3 * j * (i - j)) * (seq_t)triangle[i - 2][j - 1]);
                    row[j] = term1 + term2 - term3;
                }
            }
            triangle[i] = row;
        }
        rows = r + 1;
    }

    return triangle[r][k];
}
