/*
 * A123333  a(n) = 3^n*(Gamma(n+1/3)/Gamma(1/3) + (n-1)!).
 * https://oeis.org/A123333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{k=0}^{n-1} (3k+1) + 3^n * (n-1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 360 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

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
    /* For n < 1 the offset is not defined; return 0 for safety. */
    if (n == 0) {
        return 0;
    }

    /* Compute the product \prod_{k=0}^{n-1} (3k+1). */
    seq_t prod = 1;
    for (idx_t k = 0; k < n; ++k) {
        prod *= (seq_t)(3 * k + 1);
    }

    /* Compute the term 3^n * (n-1)! using the helper functions. */
    seq_t term2 = ipow_u64_idx(3, n);
    term2 *= factorial(n - 1);

    /* Sum the two parts to obtain a(n). */
    return prod + term2;
}
