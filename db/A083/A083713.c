/*
 * A083713  a(n) = (8^n - 1)*3/7.
 * https://oeis.org/A083713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^n - 1)*3/7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow8 = ipow_u64_idx(8, n);   /* 8^n */
    seq_t t = pow8 - 1;            /* 8^n - 1  (divisible by 7) */
    t = t / 7;                     /* (8^n - 1)/7 = a(n)/3   */
    return t * 3;                  /* 3*((8^n - 1)/7) = a(n)  */
}
