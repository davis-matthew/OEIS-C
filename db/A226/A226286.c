/*
 * A226286  [n/2]!*[(n+1)/2]!*C([n/2],5)*C([(n+1)/2],5).
 * https://oeis.org/A226286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2)! * floor((n+1)/2)! * C(floor(n/2),5) *
 *            C(floor((n+1)/2),5)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 173 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* floor divisions */
    idx_t k   = n / 2;          /* floor(n/2)   */
    idx_t k1  = (n + 1) / 2;    /* floor((n+1)/2) */

    /* If either binomial coefficient is zero, return zero immediately */
    if (k < 5 || k1 < 5)
        return 0;

    /* factorials */
    seq_t f1 = factorial(k);
    seq_t f2 = factorial(k1);

    /* binomial coefficients */
    seq_t c1 = binomial(k, 5);
    seq_t c2 = binomial(k1, 5);

    /* product of all factors */
    seq_t result = f1 * f2;
    result *= c1;
    result *= c2;

    return result;
}
