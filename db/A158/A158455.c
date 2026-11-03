/*
 * A158455  a(n) = 2^(n-1)*(n-1)!*(4*n+1).
 * https://oeis.org/A158455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1)*(n-1)!*(4*n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
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
    if (n == 0) return 0;               /* undefined for offset<1 */
    seq_t fac = factorial(n - 1);       /* (n-1)!  */
    seq_t p2  = pow2(n - 1);            /* 2^(n-1) */
    seq_t res = fac * p2;               /* 2^(n-1)*(n-1)! */
    res *= ((seq_t)4 * n + 1);          /* * (4*n+1) */
    return res;
}
