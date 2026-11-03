/*
 * A210448  Total number of different letters summed over all ternary words of
 *          length n.
 * https://oeis.org/A210448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3^n - 2^n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    /* a(0) = 0 by definition */
    if (n == 0) return (seq_t)0;
    seq_t pow3 = ipow((seq_t)3, (seq_t)n);
    seq_t pow2 = ipow((seq_t)2, (seq_t)n);
    seq_t diff = pow3 - pow2;
    return 3 * diff;
}
