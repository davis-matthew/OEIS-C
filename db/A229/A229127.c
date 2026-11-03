/*
 * A229127  Number of n-digit numbers containing the digit '0'.
 * https://oeis.org/A229127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*10^(n-1) - 9^n for n>1; a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

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
    if (n <= 1)
        return (seq_t)(n == 1 ? 1 : 0);   /* a(1)=1, undefined for n<1 */

    seq_t ten_pow  = pow10(n - 1);    /* 10^(n-1) */
    seq_t nine_pow = ipow_idx((seq_t)9, n); /* 9^n */

    return (seq_t)9 * ten_pow - nine_pow;
}
