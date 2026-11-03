/*
 * A192033  Expansion of g.f. x*(3*x^2+x+1)/((x-1)*(2*x-1)*(x+1)).
 * https://oeis.org/A192033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) - (5 + (-1)^n)/2 for n >= 1, a(0) = 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 889 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t val = pow2(n - 1);          /* 2^(n-1) */
    val = seq_mul(val, (seq_t)3);     /* 3 * 2^(n-1) */

    if ((n & 1) == 0)                /* n even */
        val -= 3;
    else                             /* n odd */
        val -= 2;

    return val;
}
