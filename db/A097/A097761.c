/*
 * A097761  Inverse of binomial transform of Whitney triangle.
 * https://oeis.org/A097761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 27, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A097761 – Inverse of binomial transform of Whitney triangle
 *  Sequence terms are the flattened (row‑major) entries of the Riordan
 *  array ((1-x*C(-x))*C(-x), x*C(-x)^2), where C(x) is the Catalan
 *  generating function.
 *
 *  Computation strategy:
 *  For n = r(r+1)/2 + k  (row r, column k 0≤k≤r)
 *      a(n) = coeff_{x^{r-k}}[C(-x)^{2k+1}]
 *             – coeff_{x^{r-k-1}}[C(-x)^{2k+2}]
 *
 *  The coefficient of x^m in C(-x)^q (q≥0) is 
 *      (-1)^m * (q/(2m+q)) * binom(2m+q, m).
 *
 *  binom(a,b) is obtained from the utility library.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* coefficient of x^m in C(-x)^q (q >= 0) */
static seq_t coeff_C_neg_pow(idx_t q, idx_t m)
{
    /* q == 0 : C(-x)^0 = 1 */
    if (q == 0)
        return (m == 0 ? (seq_t)1 : (seq_t)0);

    /* general case: (-1)^m * (q/(2m+q)) * binom(2m+q, m) */
    seq_t bin = binomial(2 * m + q, m);
    seq_t numerator = bin * (seq_t)q;
    seq_t denom     = (seq_t)(2 * m + q);
    seq_t coeff = numerator / denom;          /* exact division          */
    if (m & 1u)                               /* sign adjustment        */
        coeff = -coeff;
    return coeff;
}

seq_t a(idx_t n)
{
    /* locate row r and column k (k = n - r(r+1)/2) */
    idx_t sum = 0;        /* number of terms before current row */
    size_t r = 0;
    while (sum + (idx_t)(r + 1) <= n) {
        sum += (idx_t)(r + 1);
        ++r;
    }
    size_t k = (size_t)(n - sum);   /* 0 <= k <= r */

    /* a(n) = coeff_C_neg_pow(2k+1, r-k) - coeff_C_neg_pow(2(k+1), r-k-1) */
    seq_t term1 = coeff_C_neg_pow(2 * k + 1, r - k);

    seq_t term2 = 0;
    if (r > k)                    /* r-k-1 >= 0 only if r > k */
        term2 = coeff_C_neg_pow(2 * (k + 1), r - k - 1);

    return term1 - term2;
}
