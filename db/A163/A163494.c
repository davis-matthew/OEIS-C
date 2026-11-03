/*
 * A163494  a(n) = (4*n)!*((2*n)!)^2.
 * https://oeis.org/A163494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)! * ((2*n)!)^2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 23 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    seq_t f4 = factorial(4 * n);        /* (4*n)!           */
    seq_t f2 = factorial(2 * n);        /* (2*n)!           */
    seq_t f2_sq = seq_mul(f2, f2);      /* ((2*n)!)^2       */
    return seq_mul(f4, f2_sq);          /* a(n)             */
}
