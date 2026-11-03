/*
 * A020532  a(n) = 6th Fibonacci polynomial evaluated at 2^n.
 * https://oeis.org/A020532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (3 + 4^(1+n) + 16^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 644 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow2n  = ipow_u64_idx(2, n);       /* 2^n */
    seq_t pow4n1 = ipow_u64_idx(4, n + 1);   /* 4^(1+n) */
    seq_t pow16n = ipow_u64_idx(16, n);      /* 16^n */

    seq_t sum = 3;                       /* start with constant 3 */
    sum += pow4n1;                       /* add 4^(1+n) */
    sum += pow16n;                       /* add 16^n */

    return pow2n * sum;                  /* final product */
}
