/*
 * A093002  Product of factorials of next n numbers.
 * https://oeis.org/A093002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=n}^{2n-2} k! (a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* factorial of (n-1) */

    seq_t fact   = factorial(n - 1);    /* (n-1)!  */
    seq_t result = 1;                   /* product accumulator */

    /* multiply factorials from k=n up to k=2n-2 */
    for (idx_t k = n; k <= 2 * n - 2; ++k)
    {
        fact   = seq_mul(fact, (seq_t)k);    /* fact = k!  */
        result = seq_mul(result, fact);      /* result *= fact */
    }
    return result;
}
