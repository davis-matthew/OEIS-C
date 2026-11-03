/*
 * A082036  a(n) = (9*n^2+3*n+1) * n!.
 * https://oeis.org/A082036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*n^2 + 3*n + 1) * factorial(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    seq_t fact = factorial(n);            /* n! */
    seq_t tn   = (seq_t)n;
    seq_t coeff = (seq_t)9 * tn * tn + (seq_t)3 * tn + (seq_t)1;
    return coeff * fact;
}
