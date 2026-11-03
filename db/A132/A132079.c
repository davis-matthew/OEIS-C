/*
 * A132079  a(n) = (5^n + 3)/2.
 * https://oeis.org/A132079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n + 3)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 187 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t pow5 = ipow((seq_t)5, (seq_t)n);
    return (pow5 + (seq_t)3) / (seq_t)2;
}
