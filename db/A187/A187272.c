/*
 * A187272  a(n) = (n/4)*2^(n/2)*((1+sqrt(2))^2 + (-1)^n*(1-sqrt(2))^2).
 * https://oeis.org/A187272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n/2)*2^(n/2) for even n, and a(n) = n*2^((n+1)/2) for
 *            odd n
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 51, u64 n <= 113, u128 n <= 239,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1U) == 0) {
        idx_t k = n >> 1;
        return (seq_t)3 * (seq_t)k * pow2(k);
    } else {
        idx_t k = n >> 1;
        return (seq_t)n * pow2(k + 1);
    }
}
