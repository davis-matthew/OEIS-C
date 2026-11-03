/*
 * A000133  Number of Boolean functions of n variables.
 * https://oeis.org/A000133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2^(2^n) + (2^n-1)*2^(2^(n-1)+1))/2^(n+1) = 2^(2^n-n-1) +
 *            (2^n-1)*2^(2^(n-1)-n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_u64.h"

seq_t a(idx_t n)
{
    uint64_t e = (uint64_t)1 << n;      /* 2^n */
    seq_t t1 = pow2_u64(e - n - 1);
    seq_t t2 = (pow2_u64(n) - 1) * pow2_u64(e / 2 - n);
    return t1 + t2;
}
