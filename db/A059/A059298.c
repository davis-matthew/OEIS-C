/*
 * A059298  Triangle of idempotent numbers binomial(n,k)*k^(n-k), version 2.
 * https://oeis.org/A059298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle * T(r,k) = binom(r+1,
 *            k+1) * (k+1)^(r-k) * where r satisfies r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2 * and k = n - r*(r+1)/2 (0‑based column in row r)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 95, u64 n <= 260,
 *            u128 n <= 751, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    // Find the row r containing index n.
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        r++;
    r--;                       // now r*(r+1)/2 <= n < (r+1)*(r+2)/2

    idx_t start = (r * (r + 1)) / 2;
    idx_t k = n - start;       // column in this row (0‑based)

    // Compute binomial(r+1, k+1)
    seq_t res = binom(r + 1, k + 1);

    /* Multiply by (k+1)^(r-k).  The exponent r-k is at most r
       and r is small (≈ sqrt(2*n)), so a simple loop is fast and
       exact for any seq_t type. */
    seq_t base = (seq_t)(k + 1);
    idx_t exp   = r - k;
    for (idx_t e = 0; e < exp; ++e)
        res = res * base;

    return res;
}
