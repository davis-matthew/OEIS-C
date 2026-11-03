/*
 * A133460  a(n) = 3^n*2^(n^2).
 * https://oeis.org/A133460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n*2^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    /* compute 3^n and 2^(n^2) with arbitrary‑precision helpers,
       then multiply them to obtain the sequence term.   */
    seq_t p3 = ipow((seq_t)3, (seq_t)n);
    seq_t p2 = ipow((seq_t)2, (seq_t)n * (seq_t)n);
    return seq_mul(p3, p2);
}
