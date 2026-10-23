/*
 * A101858  Array read by antidiagonals: T(n,k) = Porta-Stolarsky star product
 *          T(n,k) = n * k = nk + floor(phi n) floor(phi k) where phi = (1 +
 *          sqrt(5))/2.
 * https://oeis.org/A101858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r*c + floor(phi*r)*floor(phi*c) where (r,c) are obtained
 *            by reading the infinite array
 *            T(i,j)=i*j+floor(phi*i)*floor(phi*j) in antidiagonal order
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_phi_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_phi_n.h"

seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that (s-1)*s/2 >= n.
       (s-1)*s/2 is the total number of terms up to and including
       diagonal s.  We start with s=2 because the first diagonal
       (sum=2) contains the very first element. */
    idx_t s = 2;
    while ( ((unsigned __int128)(s - 1) * s) / 2 < n )
        ++s;

    /* Number of terms before diagonal s. */
    idx_t prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;

    /* Position on this diagonal (1‑based). */
    idx_t k = n - prev;          /* 1 <= k <= s-1 */

    /* Convert diagonal position to matrix coordinates.
       Read order: (1,s-1), (2,s-2), ..., (s-1,1) */
    idx_t r = k;
    idx_t c = s - k;

    /* Compute T(r,c) = r*c + floor(phi*r)*floor(phi*c). */
    seq_t rseq = (seq_t)r;
    seq_t cseq = (seq_t)c;
    seq_t prod1 = rseq * cseq;
    seq_t prod2 = floor_phi_n(r) * floor_phi_n(c);
    return prod1 + prod2;
}
