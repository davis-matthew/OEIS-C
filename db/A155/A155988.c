/*
 * A155988  a(n) = (2*n + 1)*9^n.
 * https://oeis.org/A155988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*9^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 199 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t pow = ipow((seq_t)9, (seq_t)n);
    return ((seq_t)(2 * n + 1)) * pow;
}
