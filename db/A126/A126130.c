/*
 * A126130  a(n) = (n+1)^n - n!.
 * https://oeis.org/A126130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)^n – n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 345 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include helper functions */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

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

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* compute (n+1)^n */
    seq_t power = ipow((seq_t)n + 1, (seq_t)n);

    /* compute n! */
    seq_t fact = factorial(n);

    /* return the difference */
    return power - fact;
}
