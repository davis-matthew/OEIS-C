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
 * Helpers:   a883_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a883_pow2.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)1 << n;          /* 2^n */
    seq_t t1 = a883_pow2(2 * m - n);        /* 4^(2^n) / 2^n */
    seq_t t2 = a883_pow2(m + 2 - n);        /* 4*2^(2^n) / 2^n */
    seq_t t3 = a883_pow2(m) * 10;           /* 10*2^(n+2^n) / 2^n */
    seq_t num = t1 - t2 + t3 + 8;
    return num / 24;
}
