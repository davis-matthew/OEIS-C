/*
 * A092143  Cumulative product of all divisors of 1..n.
 * https://oeis.org/A092143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} k^floor(n/k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 190 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t exp = n / k;
        seq_t term = ipow_u64_idx((uint64_t)k, exp);
        result = seq_mul(result, term);
    }
    return result;
}
