/*
 * A135609  a(n) = (n!)^2 - n^n.
 * https://oeis.org/A135609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 - n^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 100 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = factorial((idx_t)n);

    /* compute n^n */
    seq_t pow_n = ipow((seq_t)n, (idx_t)n);

    /* (n!)^2 - n^n */
    seq_t result = fact * fact;
    result -= pow_n;
    return result;
}
