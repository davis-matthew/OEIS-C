/*
 * A017277  a(n) = (10*n)^9.
 * https://oeis.org/A017277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 13, u128 n <= 500,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 458 (2026-11-01)
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
    /* compute n^9 */
    seq_t n9 = ipow_idx((seq_t)n, 9);
    /* multiply by 10^9 */
    return n9 * pow10(9);
}
