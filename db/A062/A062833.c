/*
 * A062833  a(n) = 2 * n! / number of divisors of n.
 * https://oeis.org/A062833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * n! / (number of divisors of n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

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
    /* For safety, handle the off‑by‑zero case (though n ≥ 1 in the series) */
    if (n == 0)
        return 0;

    /* Compute n! exactly */
    seq_t fact = factorial(n);

    /* Numerator: 2 * n! */
    seq_t num = fact * (seq_t)2;

    /* Number of divisors of n */
    uint64_t d = num_divisors((uint64_t)n);

    /* Exact division */
    return num / (seq_t)d;
}
