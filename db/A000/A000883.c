/*
 * A000883  Number of switching networks with C(2,n) acting on the domain and
 *          AG(2,2) acting on the range.
 * https://oeis.org/A000883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(2^n) - 4*2^(2^n) + 8*2^n + 10*2^(n+2^n))/(24*2^n)
 *            computed as (2^(2^(n+1)-n) - 2^(2^n+2-n) + 10*2^(2^n) + 8)/24
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_u64.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)1 << n;          /* 2^n */
    seq_t t1 = pow2_u64(2 * m - n);        /* 4^(2^n) / 2^n */
    seq_t t2 = pow2_u64(m + 2 - n);        /* 4*2^(2^n) / 2^n */
    seq_t t3 = pow2_u64(m) * 10;           /* 10*2^(n+2^n) / 2^n */
    seq_t num = t1 - t2 + t3 + 8;
    return num / 24;
}
