/*
 * A016188  Expansion of 1/((1-8*x)*(1-12*x)).
 * https://oeis.org/A016188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*12^n - 2*8^n Equivalent recurrence: a(0)=1,
 *            a(n)=12*a(n-1)+8^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 485 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t pow12 = ipow_idx((seq_t)12, n);
    seq_t pow8  = ipow_idx((seq_t)8, n);
    return (seq_t)3 * pow12 - (seq_t)2 * pow8;
}
