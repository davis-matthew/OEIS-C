/*
 * A198275  a(n) = 17*2^n - 1.
 * https://oeis.org/A198275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*2^n - 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 996 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);   /* 2^n */
    val = val * 17;        /* 17*2^n */
    val = val - 1;         /* 17*2^n - 1 */
    return val;
}
