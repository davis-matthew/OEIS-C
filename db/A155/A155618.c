/*
 * A155618  a(n) = 7^n+4^n-1^n.
 * https://oeis.org/A155618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n+4^n-1^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t pow7 = ipow((seq_t)7, (seq_t)n);
    seq_t pow4 = ipow((seq_t)4, (seq_t)n);
    return pow7 + pow4 - (seq_t)1;
}
