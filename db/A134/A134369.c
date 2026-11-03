/*
 * A134369  a(n) = ((2n+1)!)^(n+1).
 * https://oeis.org/A134369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n+1)!)^(n+1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

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
    /* Compute (2n+1)! */
    idx_t m = 2 * n + 1;
    seq_t fact = factorial(m);

    /* Raise to the (n+1)-th power */
    return ipow_idx(fact, n + 1);
}
