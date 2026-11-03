/*
 * A130744  a(n) = n*(n+2)*n!.
 * https://oeis.org/A130744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)*n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 287 (2026-11-01)
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
    /* Compute n! */
    seq_t fac = factorial(n);

    /* a(n) = n * (n+2) * n! */
    return (seq_t)n * ((seq_t)n + 2) * fac;
}
