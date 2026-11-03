/*
 * A155648  a(n) = 10^n + 6^n - 1.
 * https://oeis.org/A155648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 6^n - 1; Offset: 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

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
    /* Compute 10^n, 6^n and sum them, then subtract 1 */
    seq_t p10 = pow10(n);           /* 10^n */
    seq_t p6  = ipow_u64_idx(6, n);     /* 6^n */
    return p10 + p6 - 1;            /* a(n) */
}
