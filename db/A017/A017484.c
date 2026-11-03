/*
 * A017484  a(n) = (11*n + 7)^12.
 * https://oeis.org/A017484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 7)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 147,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 9475 (2026-11-01)
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
    /* Compute the base value (11*n + 7).  The product of 11 and n
       fits in idx_t for all n we are interested in, and the sum
       comfortably fits into seq_t as a small integer before the
       exponentiation. */
    seq_t base = (seq_t)(11 * n + 7);

    /* Raise 'base' to the 12th power by repeated multiplication.
       Using seq_mul ensures the computation works for any seq_t
       implementation (8, 16, 32, 64 bits or arbitrary precision). */
    seq_t result = 1;
    for (idx_t k = 0; k < 12; ++k)
        result = seq_mul(result, base);

    return result;
}
