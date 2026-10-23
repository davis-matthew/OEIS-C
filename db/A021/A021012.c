/*
 * A021012  Triangle of coefficients in expansion of x^n in terms of Laguerre
 *          polynomials L_n(x).
 * https://oeis.org/A021012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, nice
 * Formula:   a(n) = (-1)^k * r! * C(r,k) * where r = ⌊(√(8n+1)‑1)/2⌋ , * k =
 *            n – r(r+1)/2 . * This flattens the triangle T(r,k) = (-1)^k r!
 *            C(r,k) row‑by‑row
 * Range:     i8 n <= 11, i16 n <= 28, i32 n <= 67, i64 n <= 174,
 *            i128 n <= 444, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* Find the row index r for a flattened index n using binary search. */
static idx_t row_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* Increase high until it exceeds the necessary row. */
    while (( (__uint128_t)high * (high + 1) / 2 ) <= n)
        high <<= 1;
    /* Binary search for the largest r with r(r+1)/2 ≤ n. */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (( (__uint128_t)mid * (mid + 1) / 2 ) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k within that row. */
    idx_t r = row_index(n);
    __uint128_t rowsum128 = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)rowsum128;

    /* Compute the triangle entry: (-1)^k * r! * C(r,k). */
    seq_t fact  = factorial(r);
    seq_t binom = binomial(r, k);
    seq_t res   = fact * binom;
    if (k & 1)            /* odd k → negative sign */
        res = -res;
    return res;
}
