/*
 * A145508  a(n+1) = a(n)^2 + 2*a(n) - 2 and a(1) = 8.
 * https://oeis.org/A145508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 8, a(n+1) = a(n)^2 + 2*a(n) - 2 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)8;
    }
    seq_t val = (seq_t)8;
    for (idx_t i = 1; i < n; ++i) {
        /* compute val = val^2 + 2*val - 2 */
        seq_t sq = seq_mul(val, val);
        seq_t two_val = seq_mul(val, (seq_t)2);
        val = sq + two_val - (seq_t)2;
    }
    return val;
}
