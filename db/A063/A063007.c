/*
 * A063007  T(n,k) = binomial(n,k)*binomial(n+k,k), 0 <= k <= n, triangle read
 *          by rows.
 * https://oeis.org/A063007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n) = T(r,k) with r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r*(r+1)/2, where T(r,k) = binomial(r,k) * binomial(r+k,k)
 *            (flattening the triangle row‑by‑row)
 * Range:     u8 n <= 13, u16 n <= 40, u32 n <= 128, u64 n <= 422,
 *            u128 n <= 1467, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* declares seq_t binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r * (r + 1) / 2) <= n)   /* n <= 10000 ⇒ at most ~140 iterations */
        ++r;
    --r;

    /* Index of the first element in row r. */
    idx_t start = r * (r + 1) / 2;

    /* Column within the row. */
    idx_t k = n - start;

    /* Compute the two binomials and their product. */
    seq_t b1 = binom(r, k);
    seq_t b2 = binom(r + k, k);

    return b1 * b2;
}
