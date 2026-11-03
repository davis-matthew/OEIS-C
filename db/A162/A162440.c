/*
 * A162440  The pg(n) sequence that is associated with the Eta triangle
 *          A160464.
 * https://oeis.org/A162440
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)!^2 * 2^floor(log2(n-1)+1) (offset 2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 15 (2026-11-01)
 * Helpers:   ilog2, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"
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
    if (n < 2) return 0;          /* below offset, undefined */

    idx_t m = n - 1;              /* compute (n-1)! */

    seq_t fact = factorial(m);    /* factorial in arbitrary precision */

    seq_t res = fact * fact;      /* (n-1)!^2 */

    idx_t k = ilog2(m) + 1;   /* exponent for 2 */
    seq_t p2 = pow2(k);              /* 2^k */

    res = res * p2;                  /* multiply by 2^k */

    return res;
}
