/*
 * A225117  Triangle read by rows, coefficients of the generalized Eulerian
 *          polynomials A_{n, 3}(x) in descending order.
 * https://oeis.org/A225117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=0..r-k} (-1)^(r-k-j) * binomial(r+1, r-k-j) *
 *            (1+3*j)^r where r is the row number such that r(r+1)/2 <= n <
 *            (r+1)(r+2)/2, and k = n - r(r+1)/2
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static idx_t integer_sqrt(idx_t v)
{
    idx_t low = 0, high = v, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            low = mid + 1;
            ans = mid;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

static seq_t pow_uint_seq(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column k of the flattened triangle. */
    idx_t t  = 8 * n + 1;
    idx_t sq = integer_sqrt(t);
    idx_t r  = (sq - 1) / 2;

    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* Compute the coefficient T(r,k) using the explicit sum. */
    seq_t res = 0;
    idx_t max_j = r - k;                     /* maximum j in the sum */
    seq_t binom = binomial(r + 1, max_j);    /* C(r+1, r-k) */

    for (idx_t j = 0; j <= max_j; ++j) {
        idx_t s = max_j - j;                 /* current binomial argument r-k-j */
        seq_t base_pow = pow_uint_seq(1 + 3 * j, r);

        if (s & 1)        /* odd exponent => negative sign */
            res -= binom * base_pow;
        else
            res += binom * base_pow;

        if (j < max_j) {  /* update binomial for next iteration: C(r+1, s-1) */
            idx_t denom = (r - s + 2);        /* = (r+1) - s + 1 */
            binom = (binom * s) / denom;
        }
    }
    return res;
}
