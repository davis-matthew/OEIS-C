/*
 * A155156  Triangle T(n, k) = 4*n*k + 2*n + 2*k, read by rows.
 * https://oeis.org/A155156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (4*r*k + 2*r + 2*k) where r = ceil((√(8*n+1)-1)/2), k = n
 *            - r*(r-1)/2, and T(r,k) = 4*r*k + 2*r + 2*k
 * Range:     u8 n <= 35, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* trival guard for out-of-range indices – not needed in typical usage */
    if (n == 0) return 0;

    /* Find the row number r (1‑based) such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((__int128)r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Column index k (1‑based). */
    idx_t prev = r * (r - 1) / 2;   /* terms before this row */
    idx_t k = n - prev;

    /* Compute the triangle entry using seq_t arithmetic. */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;

    return 4 * rr * kk + 2 * rr + 2 * kk;
}
