/*
 * A134366  a(n) = (n!)^(n-1).
 * https://oeis.org/A134366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 by definition */

    seq_t f = factorial(n);        /* compute n! */
    seq_t exp = (seq_t)(n - 1);     /* exponent (n-1), n≥1 so safe */
    return ipow(f, exp);        /* compute (n!)^(n-1) */
}
