/*
 * A216728  (2*n)!*((2*n+1)/2)^n/(2*n+1).
 * https://oeis.org/A216728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n)! * ((2*n+1)/2)^n)/(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
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
    if (n == 0)
        return (seq_t)1;

    seq_t fac   = factorial(2 * n);                     /* (2n)!          */
    seq_t powf  = ipow_idx((seq_t)(2 * n + 1), n - 1);    /* (2n+1)^(n-1)   */
    seq_t denom = pow2(n);                               /* 2^n            */
    return (fac * powf) / denom;
}
