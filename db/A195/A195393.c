/*
 * A195393  a(n) = (9*n)!.
 * https://oeis.org/A195393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)!
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 47 (2026-11-01)
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

seq_t a(idx_t n) {
    idx_t m = n * 9ULL;
    return factorial(m);
}
