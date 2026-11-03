/*
 * A225107  Number of (4*n-3)-digit 4th powers in carryless arithmetic mod 10.
 * https://oeis.org/A225107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (1/4)*(9*10^(n-1) - 2^(n-1)) + 2^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 958 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Expect n >= 1 (offset is 1). */
    idx_t exp = n - 1;

    seq_t term1 = 9 * pow10(exp);   /* 9 * 10^(n-1) */
    seq_t term2 = pow2(exp);        /* 2^(n-1) */

    seq_t delta = term1 - term2;    /* 9*10^(n-1) - 2^(n-1) */
    seq_t div   = delta / 4;        /* divide by 4 (exact) */

    return div + term2;             /* (1/4)*(...) + 2^(n-1) */
}
