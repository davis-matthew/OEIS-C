/*
 * A124051  Quasi-mirror of A062196 formatted as a triangular array.
 * https://oeis.org/A124051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r+1, r-k+1) * binomial(r+3, r-k+1) * where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2 * This enumerates
 *            the triangle T(n,k)=C(n+1,n-k+1)*C(n+3,n-k+1) row‑wise
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 159, u64 n <= 576,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the row r containing index n */
    seq_t root = floor_sqrt((seq_t)(8 * n + 1));
    idx_t r = (idx_t)((root - 1) / 2);

    /* Position k within row r */
    idx_t prev = r * (r + 1) / 2;      /* number of terms before row r */
    idx_t k    = n - prev;

    /* Compute the binomial factors */
    idx_t kk   = r - k + 1;            /* equals r-k+1 */
    seq_t term1 = binomial(r + 1, kk);
    seq_t term2 = binomial(r + 3, kk);

    return term1 * term2;
}
