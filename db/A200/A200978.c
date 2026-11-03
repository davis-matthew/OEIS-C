/*
 * A200978  Number of ways to arrange n books on 3 consecutive shelves leaving
 *          none of the shelves empty.
 * https://oeis.org/A200978
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * binomial(n-1,2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 20 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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

seq_t a(idx_t n) {
    if (n < 3)
        return 0;
    seq_t f = factorial(n);
    seq_t b = binomial(n-1, 2);
    return f * b;
}
