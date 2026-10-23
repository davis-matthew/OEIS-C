/*
 * A178111  Number triangle
 *          T(n,k)=(-1)^((n-k)/2)*C(floor(n/2),floor(k/2))*(1+(-1)^(n-k))/2.
 * https://oeis.org/A178111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(1+8n)-1)/2⌋, k = n - r(r+1)/2.
 *            T(r,k) = 0 if (r-k) is odd; otherwise T(r,k) = (-1)^((r-k)/2) *
 *            C(⌊r/2⌋, ⌊k/2⌋)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* multiplicative binomial coefficient: C(n,k) */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first, division is exact */
        res = res * ((seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* parity check: if (r-k) is odd, term is zero */
    if (((r - k) & 1) != 0) return 0;

    seq_t coeff = binom(r >> 1, k >> 1);

    /* sign = (-1)^((r-k)/2) */
    if ((((r - k) >> 1) & 1) != 0) coeff = -coeff;

    return coeff;
}
