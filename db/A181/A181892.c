/*
 * A181892  a(n) = (n!/4-1)^2.
 * https://oeis.org/A181892
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n!)/4-1)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 18 (2026-11-01)
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
    seq_t fact = factorial(n);      /* n! */
    seq_t y = fact / 4;             /* n!/4 */
    y -= 1;                         /* n!/4 - 1 */
    return y * y;                   /* square */
}
