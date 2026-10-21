/*
 * A203693  Vandermonde sequence using x^2 - xy + y^2 applied to the first n
 *          triangular numbers: 1,3,6,10,...,n(n+1)/2.
 * https://oeis.org/A203693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤i<j≤n} (T_i² – T_i T_j + T_j²) where T_k = k(k+1)/2
 *            is the k-th triangular number
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   prod_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/prod_term.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* offset is 1, behavior below is irrelevant */

    /* Allocate array of triangular numbers */
    seq_t *tri = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!tri)
        return 0;           /* insufficient memory, return 0 */

    /* Compute T_k = k(k+1)/2 using seq_t arithmetic */
    for (idx_t k = 1; k <= n; ++k)
        tri[k - 1] = (seq_t)k * ((seq_t)k + 1) / 2;

    seq_t prod = 1;

    /* Multiply over all unordered pairs (i, j) with i < j */
    for (idx_t i = 0; i + 1 < n; ++i) {
        seq_t xi = tri[i];
        for (idx_t j = i + 1; j < n; ++j) {
            seq_t term = prod_term(xi, tri[j]);
            prod *= term;
        }
    }

    free(tri);
    return prod;
}
