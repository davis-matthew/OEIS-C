/*
 * A237450  Triangle read by rows, T(n,k) = !n + (k-1)*(n-1)!, with n>=1,
 *          1<=k<=n; Position of the first n-letter permutation beginning with
 *          number k in the list of lexicographically sorted permutations
 *          A030299.
 * https://oeis.org/A237450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + Σ_{i=1}^{m-1} i! + (n - m(m-1)/2 - 1) * (m-1)! where
 *            m = smallest integer with m(m+1)/2 ≥ n. This computes the
 *            position of the first n‑permutation beginning with each first
 *            element in the lexicographically sorted list of all permutations
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 86, u64 n <= 217,
 *            u128 n <= 596, big any n
 * Helpers:   _tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_tri.h"

seq_t a(idx_t n)
{
    /* Find the row length m (triangular number index). */
    idx_t m = 1;
    while (_tri(m) < (unsigned __int128)n)
        ++m;

    /* Offset within the row: k-1. */
    idx_t prev_tri = (idx_t)_tri(m - 1);
    idx_t offset = n - prev_tri - 1;          /* 0 ≤ offset < m */

    /* Compute (m-1)! and Σ_{i=1}^{m-1} i! . */
    seq_t fact = 1;   /* (m-1)!  (starts as 1! = 1 when m=2, stays 1 when m=1)   */
    seq_t sum  = 0;   /* Σ_{i=1}^{m-1} i! */

    for (idx_t i = 1; i < m; ++i) {
        fact *= (seq_t)i;
        sum  += fact;
    }

    seq_t base  = 1 + sum;                     /* 1 + Σ_{i=1}^{m-1} i! */
    seq_t result = base + (seq_t)offset * fact;/* + (k-1)*(m-1)! */

    return result;
}
