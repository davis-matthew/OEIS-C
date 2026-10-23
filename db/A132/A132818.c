/*
 * A132818  The matrix product A127773 * A001263 of infinite lower triangular
 *          matrices.
 * https://oeis.org/A132818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = flattened entries of the lower triangular matrix T(n,k) =
 *            1/2 * (n+1-k) * C(n+1,k) * C(n,k-1)
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }
    /* Position within the row (1‑based) */
    idx_t k = n - (cum - r);

    /* Compute the binomial coefficients */
    seq_t c1 = binom_n_k(r + 1, k);      /* C(r+1,k) */
    seq_t c2 = binom_n_k(r, k - 1);      /* C(r,k-1) */
    seq_t mult = (seq_t)(r + 1 - k);      /* (r+1-k) */

    /* One of mult, c1, c2 must be even – divide the even factor by 2 first */
    if ((mult & 1) == 0) {
        mult = mult / 2;
    } else if ((c1 & 1) == 0) {
        c1 = c1 / 2;
    } else {
        c2 = c2 / 2;
    }

    /* Multiply the three factors */
    seq_t val = c1 * c2;
    val = val * mult;
    return val;
}
