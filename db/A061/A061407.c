/*
 * A061407  Denominators of coefficients of even powers of x of the expansion
 *          of Bessel function J_8(x).
 * https://oeis.org/A061407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n+8) * n! * (n+8)!
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 211 (2026-11-01)
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
    /* Compute n! */
    seq_t res = factorial(n);

    /* Multiply by (n+8)! */
    res *= factorial(n + (idx_t)8);

    /* Multiply by 2^(2n+8) */
    idx_t exp = (n << 1) + 8U;          /* exp = 2*n + 8, all in idx_t */
    res *= pow2(exp);

    return res;
}
