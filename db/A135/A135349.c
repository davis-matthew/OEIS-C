/*
 * A135349  a(n) = 6^n*5^(n^2).
 * https://oeis.org/A135349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n*5^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-01)
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
    seq_t pow6 = ipow_idx((seq_t)6, n);
    seq_t pow5 = ipow_idx((seq_t)5, n * n);
    return seq_mul(pow6, pow5);
}
