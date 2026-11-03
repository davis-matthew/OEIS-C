/*
 * A170734  Expansion of g.f.: (1+x)/(1-14*x).
 * https://oeis.org/A170734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 15 * 14^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 791 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    } else {
        seq_t pow = ipow_idx((seq_t)14, n - 1);
        return seq_mul((seq_t)15, pow);
    }
}
