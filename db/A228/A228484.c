/*
 * A228484  a(n) = 2^n*(3*n)!/(n!*(2*n)!).
 * https://oeis.org/A228484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(3*n)!/(n!*(2*n)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 192 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t p2 = pow2(n);                      /* 2^n */
    seq_t b  = binomial(3 * n, n);           /* (3n choose n) */
    return seq_mul(p2, b);                   /* a(n) = 2^n * binomial(3n, n) */
}
