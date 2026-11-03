/*
 * A204841  (2n)! - 2^n*n!.
 * https://oeis.org/A204841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! - 2^n * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

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
    /* compute (2n)! */
    idx_t two_n = n << 1;                     /* 2*n, safe for unsigned */
    seq_t fact_2n = factorial(two_n);

    /* compute n! and 2^n separately */
    seq_t fact_n  = factorial(n);
    seq_t pow2_n  = pow2(n);

    /* compute 2^n * n! */
    seq_t prod = pow2_n * fact_n;

    /* a(n) = (2n)! - 2^n * n! */
    return fact_2n - prod;
}
