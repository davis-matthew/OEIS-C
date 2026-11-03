/*
 * A217994  a(n) = 2^((2 + n + n^2)/2).
 * https://oeis.org/A217994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^((n^2 + n + 2)/2) = 2^( n*(n+1)/2 + 1 )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 75 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the exponent: n(n+1)/2 + 1 */
    seq_t exp = (seq_t)n * ((seq_t)n + 1) / 2 + 1;
    /* Return 2 raised to that exponent */
    return pow2((idx_t)exp);
}
