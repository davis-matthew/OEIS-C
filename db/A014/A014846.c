/*
 * A014846  a(n) = 2^(n-1) - n*(n+1)/2.
 * https://oeis.org/A014846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(n-1) - n*(n+1)/2
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 1000 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    /* Compute 2^(n-1) in arbitrary precision */
    seq_t pow2 = ipow((seq_t)2, (seq_t)(n - 1));

    /* Compute the triangular number n*(n+1)/2 */
    seq_t tri = ((seq_t)n) * ((seq_t)n + 1) / 2;

    return pow2 - tri;
}
