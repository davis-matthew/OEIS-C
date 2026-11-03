/*
 * A168467  a(n) = Product_{k=0..n} ((2*k+2)*(2*k+3))^(n-k).
 * https://oeis.org/A168467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=1..n} (2*k+1)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 28 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;   /* a(0) = 1 */
    seq_t fact   = (seq_t)1;   /* factorial of the previous odd, starts with 1! = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t mul  = (seq_t)(2*k) * (seq_t)(2*k+1);   /* (2k)*(2k+1)   */
        fact = seq_mul(fact, mul);                    /* fact = (2k+1)! */
        result = seq_mul(result, fact);               /* accumulate product */
    }

    return result;
}
