/*
 * A176647  Triangle T(n, k) = f(n, k) + f(n, n-k) - f(n, n), where f(n, k) =
 *          binomial(n*(3*n-1)/2 + k, k), read by rows.
 * https://oeis.org/A176647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = binomial(r*(3*r-1)/2 + k, k) + binomial(r*(3*r-1)/2 +
 *            (r-k), r-k) - binomial(r*(3*r-1)/2 + r, r), where r and k are
 *            determined from the index n by n = r*(r+1)/2 + k (0 ≤ k ≤ r).
 *            This reads the triangle T(r,k) = f(r,k)+f(r,r-k)-f(r,r)
 *            row‑by‑row
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 28, i64 n <= 78, i128 n <= 231,
 *            big any n
 * Helpers:   binomial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    /* k is the offset within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Base value b = r*(3*r - 1)/2 used in all binomials */
    idx_t b = r * (3 * r - 1) / 2;

    /* Compute the three required binomials.  The binomial function
       takes idx_t arguments, so the computed indices are guaranteed
       to fit in uint64_t for n up to at least 10 000. */
    seq_t f_r_k   = binomial(b + k,   k);
    seq_t f_r_nk  = binomial(b + (r - k), r - k);
    seq_t f_r     = binomial(b + r,   r);

    return f_r_k + f_r_nk - f_r;
}
