/*
 * A165643  Number of ways to assemble an n-cube from 2n labeled (n-1)-cubes
 *          with labeled vertices, where left-handed and right-handed
 *          counterparts are considered equivalent.
 * https://oeis.org/A165643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n-2)!!)^ (2n-1) * (2n-1)!
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
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
    /* n >= 1 (sequence is defined from offset 1) */
    idx_t nn = (idx_t)n;

    /* (n-1)! */
    seq_t fact_nm1 = factorial(nn - 1);

    /* (2n-1)! */
    seq_t fact_2n_1 = factorial(2 * nn - 1);

    /* 2^(n-1) */
    seq_t twos = ipow_idx((seq_t)2, nn - 1);

    /* (2n-2)!! = 2^(n-1) * (n-1)! */
    seq_t base = twos * fact_nm1;

    /* exponent (2n-1) */
    idx_t exp = 2 * nn - 1;

    /* ((2n-2)!!)^(2n-1) */
    seq_t base_pow = ipow_idx(base, exp);

    /* final result */
    seq_t result = base_pow * fact_2n_1;

    return result;
}
