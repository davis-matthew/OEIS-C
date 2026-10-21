/*
 * A096041  Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^7-M)/6,
 *          where M is the infinite lower Pascal's triangle matrix, 1<=k<=n.
 * https://oeis.org/A096041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A096041: Triangle read by rows.
    T(r,k) = binomial(r, k-1) * (7^(r-k+1) - 1) / 6,
    where r is the row (1‑based) and 1 ≤ k ≤ r.
    The sequence a(n) is obtained by reading this triangle row by
    row: a(1) = T(1,1), a(2) = T(2,1), a(3) = T(2,2), a(4) = T(3,1), …
*/

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of terms up to
       row r is at least n – solve r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((r * (r + 1ULL)) / 2 < n) {
        ++r;
    }

    /* Column k (1‑based) in row r. */
    idx_t prev_terms = (r - 1) * r / 2;   /* terms before row r */
    idx_t k = n - prev_terms;            /* 1 ≤ k ≤ r */

    /* Compute the triangle entry. */
    seq_t binom = binomial(r, k - 1);          /* C(r, k-1) */
    idx_t exp   = r - k + 1;                   /* exponent for 7 */
    seq_t pow7  = pow_uint(7, exp);            /* 7^exp */
    seq_t term  = (pow7 - 1) / 6;              /* (7^exp - 1)/6 */

    return binom * term;                       /* exact product in seq_t */
}
