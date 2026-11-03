/*
 * A036562  a(n) = 4^(n+1) + 3*2^n + 1.
 * https://oeis.org/A036562
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n+1) + 3*2^n + 1 (offset -1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 992 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The first term (offset -1) is defined separately. */
    if (n == -1)
        return (seq_t)1;

    /* For n >= 0 use the closed‑form expression. */
    return pow4(n + 1) + (seq_t)3 * pow2(n) + (seq_t)1;
}
