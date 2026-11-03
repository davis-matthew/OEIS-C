/*
 * A016152  a(n) = 4^(n-1)*(2^n-1).
 * https://oeis.org/A016152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1)*(2^n-1) (a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 133 (2026-11-01)
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
    if (n == 0)
        return (seq_t)0;

    seq_t pow2n   = pow2(n);     /* 2^n   */
    seq_t pow4n1  = pow4(n-1);   /* 4^(n-1) */

    return pow4n1 * (pow2n - (seq_t)1);
}
