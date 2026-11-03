/*
 * A066118  a(n) = n!*(3*n-1)/2.
 * https://oeis.org/A066118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(3*n-1)/2 (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 100 (2026-11-01)
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
    if (n == 0) return 0;                /* undefined for n=0, but safe guard */
    seq_t f = factorial(n);              /* compute n! */
    seq_t factor = (seq_t)3 * (seq_t)n - 1; /* compute (3*n-1) */
    f *= factor;                         /* multiply */
    f /= 2;                               /* divide by 2 (exact) */
    return f;
}
