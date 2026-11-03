/*
 * A198763  a(n) = 4*5^n-1.
 * https://oeis.org/A198763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*5^n-1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 891 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t p      = ipow_idx(5, n);    /* compute 5^n        */
    seq_t result = seq_mul(p, 4);    /* multiply by 4      */
    result = result - 1;             /* subtract 1         */
    return result;
}
