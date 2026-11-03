/*
 * A127610  a(n) = floor(( (n+1)/2 )^n) - n!.
 * https://oeis.org/A127610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(((n+1)/2)^n) - n!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 379 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

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
    /* For very small seq_t types (≤64 bits) we use a 128‑bit intermediate
       to avoid overflowing while computing (n+1)^n.  When seq_t is
       larger (or an arbitrary‑precision class) we rely on the provided
       big_pow routine. */
    seq_t power_term;
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 num = 1;
        uint64_t base = (uint64_t)(n + 1);
        for (idx_t i = 0; i < n; ++i)
            num *= base;
        /* Division by 2^n is a right shift; truncation gives floor. */
        num >>= n;
        power_term = (seq_t)num;
    } else {
        power_term = ipow((seq_t)(n + 1), (seq_t)n) / ipow((seq_t)2, (seq_t)n);
    }
    seq_t fact = factorial(n);
    return power_term - fact;
}
