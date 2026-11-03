/*
 * A017107  a(n) = (8*n+3)^7.
 * https://oeis.org/A017107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 70, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 9975 (2026-11-01)
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
    seq_t base = 8 * (seq_t)n + (seq_t)3;  /* (8*n+3) */
    seq_t res  = (seq_t)1;                /* accumulator for base^7 */

    for (int i = 0; i < 7; ++i)
    {
        res = seq_mul(res, base);         /* multiply using the provided helper */
    }
    return res;
}
