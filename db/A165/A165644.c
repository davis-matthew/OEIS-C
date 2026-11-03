/*
 * A165644  Number of ways to assemble an n-simplex from n+1 labeled
 *          (n-1)-simplices with labeled vertices, where left-handed and
 *          right-handed counterparts are considered distinct.
 * https://oeis.org/A165644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (n!)^(n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-01)
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

/* Computes a(n) = 2 * (n!)^(n+1) for n >= 1. 
   If n < 1, returns 0 (offset is 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Compute n! */
    seq_t fact = factorial(n);

    /* Compute (n!)^(n+1) */
    seq_t pow_fact = ipow_idx(fact, n + 1);

    /* Multiply by 2 */
    return pow_fact * (seq_t)2;
}
