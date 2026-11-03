/*
 * A183625  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock summing
 *          to 4.
 * https://oeis.org/A183625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(4 + 2^(n+2) + 3^n)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 194 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t pow2 = ipow_idx((seq_t)2, n);   /* 2^n */
    seq_t pow3 = ipow_idx((seq_t)3, n);   /* 3^n */
    seq_t res  = (seq_t)12;              /* 12 = 3*4 */
    res += pow2 * (seq_t)12;             /* 12 * 2^n = 3*4*2^n */
    res += pow3 * (seq_t)3;              /* 3 * 3^n */
    return res;                          /* a(n) */
}
