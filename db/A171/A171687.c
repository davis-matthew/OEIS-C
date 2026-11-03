/*
 * A171687  a(n) = (10^n)^4+(10^n+1)^4.
 * https://oeis.org/A171687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n)^4 + (10^n + 1)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 191 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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
    /* pow10(n) yields 10^n as seq_t. */
    seq_t t = pow10(n);

    /* (10^n)^4 */
    seq_t t4 = ipow_idx(t, 4);

    /* (10^n + 1)^4 */
    seq_t t1plus1 = t + 1;
    seq_t u4 = ipow_idx(t1plus1, 4);

    /* a(n) = t4 + u4 */
    return t4 + u4;
}
