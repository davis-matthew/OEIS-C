/*
 * A032261  Number of bracelets with n labeled beads of 3 colors.
 * https://oeis.org/A032261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * (n-1)! / 2 for n ≥ 3; special cases: a(0) = 1, a(1)
 *            = 3, a(2) = 9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 170 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    seq_t pow3_n = pow3(n);          /* 3ⁿ */
    seq_t fact   = factorial(n - 1); /* (n-1)! */
    seq_t result = pow3_n * fact;
    result /= (seq_t)2;              /* divide by 2 */
    return result;
}
