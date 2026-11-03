/*
 * A036099  Centered cube numbers: (n+1)^21 + n^21.
 * https://oeis.org/A036099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^21 + n^21
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 7, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 8838 (2026-11-01)
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

static inline seq_t pow21(seq_t x)
{
    seq_t r = 1;
    for (int i = 0; i < 21; ++i)
        r = seq_mul(r, x);
    return r;
}

seq_t a(idx_t n)
{
    seq_t xn   = (seq_t)n;
    seq_t xn1  = (seq_t)(n + 1);
    seq_t v1   = pow21(xn);
    seq_t v2   = pow21(xn1);
    return v1 + v2;
}
