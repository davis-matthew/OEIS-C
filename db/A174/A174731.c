/*
 * A174731  Triangle T(n, k, q) = (1-q^n)*(1/k)*binomial(n-1, k-1)*binomial(n,
 *          k-1) - (1-q^n) + 1, for q = 2, read by rows.
 * https://oeis.org/A174731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th element of the triangle * T(n,k,2) =
 *            (1-2^n)*(A001263(n,k)-1)+1, * read by rows (row 1: k=1; row 2:
 *            k=1…2; …)
 * Range:     i8 n <= 11, i16 n <= 30, i32 n <= 95, i64 n <= 310,
 *            i128 n <= 1100, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Find the row r (>=1) that contains the global index n (>=1). */
static idx_t row_of_idx(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* Expand hi until the triangular number >= n. */
    while (((__int128)hi * (hi + 1) / 2) < n) {
        hi <<= 1;
    }
    /* Binary search between lo and hi (exclusive lo). */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((__int128)mid * (mid + 1) / 2) >= n) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return hi;               /* row number r */
}

seq_t a(idx_t n)
{
    /* Determine (row r, column k) from the global index n. */
    idx_t r = row_of_idx(n);
    idx_t prev_rows = ((idx_t)r - 1) * r / 2;
    idx_t k = n - prev_rows;               /* 1 ≤ k ≤ r */

    /* Compute Narayana number A001263(r,k) = binom(r,k)*binom(r,k-1)/r. */
    seq_t bin_r_k   = binomial(r, k);          /* C(r,k)   */
    seq_t bin_r_km1 = binomial(r, k - 1);      /* C(r,k-1) */
    seq_t numer = bin_r_k * bin_r_km1;         /* product in seq_t   */
    seq_t A = numer / (seq_t)r;                /* division in seq_t  */

    /* 2^r and the final formula. */
    seq_t two_pow_r = pow2(r);
    seq_t one = (seq_t)1;
    seq_t res = (one - two_pow_r) * (A - one) + one;
    return res;
}
