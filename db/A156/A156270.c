/*
 * A156270  a(n) = 8^n*Catalan(n).
 * https://oeis.org/A156270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n * Catalan(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 189 (2026-11-01)
 * Helpers:   catalan, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t pow8 = ipow_u64_idx(8, n);
    seq_t cat  = catalan(n);
    return seq_mul(pow8, cat);
}
