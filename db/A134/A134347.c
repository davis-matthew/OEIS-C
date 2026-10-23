/*
 * A134347  A007318^(-1) * A134346.
 * https://oeis.org/A134347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(j+1) * C(r,j) if j<r * a(n) = 2^(r+1) - 1 if j==r *
 *            where r = floor((sqrt(8*n+1)-1)/2) and j = n - r*(r+1)/2 * (i.e.
 *            a(n) is the triangle entry T(r,j) read by antidiagonals)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: integer power of 2, power 2^exp */
static seq_t pow2_u64(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= 2;
    }
    return res;
}

/* Helper: binomial coefficient C(n,k) computed multiplicatively.
 * This works for all seq_t types that support +,-,*,/ operators. */
static seq_t binom_n_k(idx_t n, idx_t k) {
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ( (seq_t)(n - k + i) );
        res = res / ( seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (n >= r * (r + 1) / 2)
        ++r;
    --r;                           /* r = row index */

    idx_t prev = r * (r + 1) / 2;   /* number of terms before row r */
    idx_t j = n - prev;             /* column index in that row */

    if (j < r) {
        /* T(r,j) = 2^(j+1) * C(r,j) */
        seq_t p = pow2_u64(j + 1);
        seq_t c = binom_n_k(r, j);
        return p * c;
    } else { /* j == r */
        /* T(r,r) = 2^(r+1) - 1 */
        seq_t p = pow2_u64(r + 1);
        return p - 1;
    }
}
