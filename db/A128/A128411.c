/*
 * A128411  Coefficient array for orthogonal polynomials defined by C(2n,n).
 * https://oeis.org/A128411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = coefficient of x^k in the orthogonal polynomial * p(n,x)
 *            defined by: p(0,x)=1, * p(1,x)=x-2, * p(n,x) = (2x-4)p(n-1,x) -
 *            4p(n-2,x). * The coefficients are given by * T(n,k) = (
 *            binom(n+k , n-k) (-1)^(n-k) * - binom(n+k-1 , n-k-1)
 *            (-1)^(n-k-1) ) * * ( 2^(n-1) + 0^n / 2 ), * where 0^0 = 1. F...
 * Range:     i8 n <= 11, i16 n <= 37, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find row r and column k */
    idx_t r = 0;
    idx_t cum = 0;                /* number of terms before current row */
    while (n >= cum + (r + 1)) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;            /* offset within the row */

    /* factor (2^(r-1) + 0^r / 2) */
    seq_t factor;
    if (r == 0) {
        factor = 1;               /* 1/2 + 1/2 = 1 for r = 0 */
    } else {
        factor = pow2(r - 1);     /* 2^(r-1) for r > 0 */
    }

    /* term1 = binom(r+k , r-k) * (-1)^(r-k) */
    seq_t bin1 = binomial(r + k, r - k);
    seq_t term1 = ((r - k) & 1) ? -bin1 : bin1;

    /* term2 = binom(r+k-1 , r-k-1) * (-1)^(r-k-1) */
    seq_t term2 = 0;
    if (r > k) {                  /* r-k-1 >= 0 when r > k */
        seq_t bin2 = binomial(r + k - 1, r - k - 1);
        term2 = ((r - k - 1) & 1) ? -bin2 : bin2;
    }

    /* T(r,k) = term1 - term2 */
    seq_t term = term1 - term2;

    /* a(n) = T(r,k) * factor */
    return term * factor;
}
