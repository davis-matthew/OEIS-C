/*
 * A121757  Triangle read by rows: multiply Pascal's triangle by
 *          1,2,6,24,120,720,... = A000142.
 * https://oeis.org/A121757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the nth entry of the triangle with entries binom(n,k) *
 *            (k+1)! read by rows. For 0 ≤ n < ∞
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 88, u64 n <= 227,
 *            u128 n <= 624, big any n
 * Validated: standard, pass, 69 terms through n = 5133 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n) {
    /* locate the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2   */
    idx_t r = (idx_t)((sqrtl(1.0L + 8.0L * (long double)n) - 1.0L) / 2.0L);
    /* correct possible floating‑point inaccuracy */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;            /* column index in this row   */
    seq_t binom = binomial(r, k);             /* A007318(n,k)                */
    seq_t fact  = factorial(k + 1);           /* (k+1)!                      */
    return binom * fact;                      /* a(n) value                   */
}
