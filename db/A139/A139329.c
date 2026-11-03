/*
 * A139329  a(n) = (factorial of the number of 0's in the binary expansion of
 *          n).
 * https://oeis.org/A139329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = factorial(number of 0's in the binary expansion of n)
 * Range:     u8 n <= 63, u16 n <= 511, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 113 terms through n = 8011 (2026-11-01)
 * Helpers:   ilog2, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ilog2.h"
#include "../utils/popcount.h"

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
    /* Number of bits needed to represent n.
       For n == 0, we consider the single bit '0', so bits = 1. */
    idx_t bits = (n == 0) ? 1ULL : (ilog2(n) + 1ULL);

    /* Count of 1-bits and consequently the count of 0-bits. */
    idx_t ones  = popcount(n);
    idx_t zeros = bits - ones;

    /* Return factorial of the number of zeros. */
    return factorial(zeros);
}
