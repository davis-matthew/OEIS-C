/*
 * A192030  Square array read by antidiagonals: W(n,p) (n>=1, p>=1) is the
 *          Wiener index of the graph G(n,p) obtained in the following way:
 *          consider n copies of a star tree with p-1 edges, add a vertex to
 *          their union, and connect this vertex with the roots of the star
 *          trees.
 * https://oeis.org/A192030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = W(i,j) where W(n,p) = n*p*(2*n*p-n-p+1) read along * the
 *            antidiagonals of the square array W(n,p)
 * Range:     u8 n <= 23, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the n‑th term of OEIS sequence A192030. */
seq_t a(idx_t n)
{
    idx_t k = n;                     /* sequence index (1‑based) */

    /* Find the diagonal d with k ≤ d*(d+1)/2.
       Binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low  = 1;
    idx_t high = 2;
    while (((unsigned __int128)high * (high + 1)) / 2 < k)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= k)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low; /* diagonal length (also the sum n+p‑1 of its entries) */

    /* Number of terms before this diagonal */
    idx_t prev = ((unsigned __int128)d * (d - 1)) / 2;

    /* Offset within the diagonal (0‑based). */
    idx_t offset = k - prev - 1;

    /* Coordinates (i,j) in the square array. */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Compute W(i,j) = i*j*(2*i*j - i - j + 1). */
    seq_t Si    = (seq_t)i;
    seq_t Sj    = (seq_t)j;
    seq_t prod  = Si * Sj;
    seq_t inner = prod * 2 - Si - Sj + 1;
    return prod * inner;
}
