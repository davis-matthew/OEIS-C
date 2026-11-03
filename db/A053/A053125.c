/*
 * A053125  Triangle of coefficients of Chebyshev's U(n,2*x-1) polynomials
 *          (exponents of x in decreasing order).
 * https://oeis.org/A053125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 28, i32 n <= 94, i64 n <= 356,
 *            i128 n <= 1325, big any n
 * Validated: standard, pass, 66 terms through n = 1260 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A053125 – coefficients of Chebyshev U(n,2x-1) flattened by rows.
 * a(n) = coefficient a(r,c) where r = floor((sqrt(8n+1)-1)/2),
 *       c = n - r(r+1)/2, and
 *       a(r,c) = (-1)^c * 4^(r-c) * binomial(2r+1-c , c).
 * The implementation uses a binary search to find the row r and
 * the utility functions `binom` and `pow4` to compute the binomial
 * coefficient and the power of 4 while staying within `seq_t` precision.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Find largest r such that r*(r+1)/2 <= n (row index). */
static idx_t triangular_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;  /* upper bound cannot exceed n          */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;              /* ceil((low+high)/2) */
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column c of the flattened triangle */
    idx_t r = triangular_row(n);
    __uint128_t tri_r = (__uint128_t)r * (r + 1) / 2;
    idx_t c = n - (idx_t)tri_r;   /* n < (r+1)(r+2)/2 so 0 <= c <= r */

    /* Compute a(r,c) = (-1)^c * 4^(r-c) * binomial(2r+1-c , c)  */
    idx_t two_r_plus_one_minus_c = 2 * r + 1 - c;   /* positive */
    seq_t binom_val = binomial(two_r_plus_one_minus_c, c);
    seq_t pow4_val  = pow4(r - c);

    seq_t result = binom_val * pow4_val;
    if (c & 1)  /* odd c -> negative sign */
        result = -result;

    return result;
}
