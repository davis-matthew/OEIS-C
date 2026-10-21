/*
 * A203955  Symmetric matrix based on (1,2,3,1,2,3,1,2,3...), by
 *          antidiagonals.
 * https://oeis.org/A203955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the entry M[i,j] of the infinite symmetric matrix M = Tᵀ
 *            · T, where T has rows (0…0 s), s = (1,2,3,1,2,3,…). The n‑th
 *            term is obtained by enumerating M by antidiagonals. M[i,j] =
 *            Σ_{k=1}^{min(i,j)} ((i‑k) mod 3 + 1) * ((j‑k) mod 3 + 1)
 * Range:     u8 n <= 44, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t triangular(idx_t d)
{
    /* T(d) = d*(d+1)/2, computed in seq_t to avoid overflow for large d */
    return ((seq_t)d * (seq_t)(d + 1)) / 2;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal d containing the n-th element:
       T(d‑1) < n ≤ T(d), where T(d) = d*(d+1)/2. */
    idx_t low = 1;
    idx_t high = n;               /* d ≤ n, high is safe */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                /* diagonal index (1‑based) */

    /* Number of elements before diagonal d */
    seq_t prev = triangular(d - 1);

    /* Position of n within diagonal d (1‑based) */
    idx_t pos = (idx_t)(n - (idx_t)prev);   /* 1 ≤ pos ≤ d */

    /* Coordinates in the matrix: i increases along the diagonal */
    idx_t i = pos;
    idx_t j = d - pos + 1;

    /* Compute the matrix entry M[i,j] */
    idx_t min_ij = (i < j) ? i : j;
    seq_t sum = 0;

    for (idx_t k = 1; k <= min_ij; ++k) {
        seq_t s1 = ((seq_t)((i - k) % 3)) + 1;   /* s(i‑k+1) */
        seq_t s2 = ((seq_t)((j - k) % 3)) + 1;   /* s(j‑k+1) */
        sum += s1 * s2;
    }

    return sum;
}
