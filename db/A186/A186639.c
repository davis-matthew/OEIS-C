/*
 * A186639  a(n) = n!/2-(-2)^(n-2)*(n-2).
 * https://oeis.org/A186639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!/2 - (-2)^(n-2)*(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

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
    /* Handle the base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 0;

    /* n >= 2 */
    seq_t fact = factorial(n);
    fact /= 2;

    idx_t exp = n - 2;
    seq_t pow2_val = pow2(exp);
    seq_t term = pow2_val * (seq_t)exp;           /* 2^(n-2)*(n-2) */

    if (exp & 1) {                                /* odd exponent */
        fact += term;          /* subtract a negative value => add */
    } else {                                /* even exponent */
        fact -= term;          /* subtract a positive value */
    }

    return fact;
}
