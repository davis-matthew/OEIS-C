/*
 * A062139  Coefficient triangle of generalized Laguerre polynomials
 *          n!*L(n,2,x) (rising powers of x).
 * https://oeis.org/A062139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the row‑major flatten of the triangle T(r,c) = (-1)^c *
 *            r! * binom(r+2, r-c) / c! where r is the row index and c the
 *            column index. For an index n (offset 0) we have: r = max integer
 *            such that r*(r+1)/2 <= n c = n - r*(r+1)/2 Then a(n) = (-1)^c *
 *            (r! / c!) * binom(r+2, c+2)
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 65, i64 n <= 172, i128 n <= 498,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t binomial_small(idx_t a, idx_t k) {
    /* Compute binomial(a,k) using multiplicative formula. */
    if (k > a - k) k = a - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(a - k + i);   /* numerator part */
        res /= (seq_t)i;             /* exact division */
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find row r */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* Column c */
    idx_t c = n - r * (r + 1) / 2;

    /* Sign */
    seq_t sign = (c & 1) ? -1 : 1;

    /* Factorial ratio r! / c! = product from c+1 to r */
    seq_t fac_ratio = 1;
    for (idx_t i = c + 1; i <= r; ++i) {
        fac_ratio *= (seq_t)i;
    }

    /* Binomial part binom(r+2, c+2) */
    seq_t binom_part = binomial_small(r + 2, c + 2);

    /* Combine */
    seq_t result = fac_ratio * binom_part;
    result *= sign;
    return result;
}
