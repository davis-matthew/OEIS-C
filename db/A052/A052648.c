/*
 * A052648  Expansion of e.g.f. 5*x/(1-x).
 * https://oeis.org/A052648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5 * n! (a(0) = 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
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
    if (n == 0)
        return 0;

    /* Compute n! iteratively (factorial is available but
       this loop keeps the dependency minimal). */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    return fact * 5;
}
