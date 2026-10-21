/*
 * A069466  Triangle T(n, k) of numbers of square lattice walks that start and
 *          end at origin after 2*n steps and contain exactly k steps to the
 *          east, possibly touching origin at intermediate stages.
 * https://oeis.org/A069466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 58, u64 n <= 179, u128 n <= 643,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(2r,r) * binomial(r,k)^2
 * where r = floor((sqrt(8n+1)-1)/2) and k = n - r*(r+1)/2
 * (this selects the row r and column k of the triangle T(r,k).
 */
#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the row index r.  r is the largest integer such that
     * r*(r+1)/2 <= n.  This is equivalent to floor((sqrt(8*n+1)-1)/2). */
    long double disc = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((disc - 1.0L) / 2.0L);

    /* Make sure r satisfies the inequality.  For very large n the
     * floating‑point round‑off might produce the wrong floor, so
     * adjust if needed. */
    if ((idx_t)((r + 1) * (r + 2) / 2) <= n) {
        r++;
    }

    /* Column index k within row r. */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute the binomial coefficients using the provided helper.
     * The helper uses 128‑bit intermediates to avoid overflow for
     * the 64‑bit build; the product may overflow seq_t when the
     * result does not fit, as required by the specification. */
    seq_t c2r = binomial(2 * r, r);   /* C(2r, r) */
    seq_t crk = binomial(r, k);       /* C(r, k)  */

    return c2r * crk * crk;           /* T(r,k) */
}
