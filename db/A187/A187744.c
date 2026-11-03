/*
 * A187744  Numbers whose digital sum is a triangular number.
 * https://oeis.org/A187744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9098 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187744: Numbers whose digital sum is a triangular number.
   a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 6, …
   The function returns the nth term (n ≥ 1). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

static bool is_triangular_sum(seq_t sum) {
    /* Triangular numbers up to 171 (9*19) */
    const seq_t tri[19] = {
        0, 1, 3, 6, 10, 15, 21, 28, 36, 45,
        55, 66, 78, 91, 105, 120, 136, 153, 171
    };
    for (size_t i = 0; i < 19; ++i) {
        if (tri[i] == sum) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cur = 0;
    while (1) {
        seq_t d = digit_sum(cur);
        if (is_triangular_sum(d)) {
            ++count;
            if (count == n) {
                return cur;
            }
        }
        /* Use seq_t increment; for large seq_t values, this works as defined. */
        cur++;
    }
}
