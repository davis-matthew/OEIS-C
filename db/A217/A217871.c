/*
 * A217871  a(n)=b(n,1) where b(0,m)=m, b(n,m)=b(floor(n/4),m*2).
 * https://oeis.org/A217871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n,1) where * b(0,m) = m * b(n,m) = b(floor(n/4), m*2) *
 *            This is equivalent to 2^k where k = floor(log_4(n)) + 1 for n>0,
 *            * and k = 0 for n = 0
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t t = n;
    unsigned int steps = 0;
    while (t != 0) {
        t /= 4;
        ++steps;
    }

    return pow2((idx_t)steps);
}
