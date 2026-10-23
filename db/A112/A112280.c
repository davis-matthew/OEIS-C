/*
 * A112280  Coefficients, read modulo 9, of the cube of q-series (q;q)_oo.
 * https://oeis.org/A112280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is not a triangular number n=m(m+1)/2; otherwise
 *            a(n) = (-1)^m * (2*m+1) (mod 9)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   u128_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_sqrt.h"

seq_t a(idx_t n)
{
    __uint128_t s = (__uint128_t)n * 8 + 1;
    uint64_t r = u128_sqrt(s);
    if ((__uint128_t)r * r != s) return (seq_t)0;          // not triangular
    uint64_t m = (r - 1) >> 1;                            // solve m(m+1)/2 = n
    uint64_t val = ((m << 1) + 1) % 9;                    // (2*m+1) mod 9
    if (m & 1) val = (9 - val) % 9;                       // apply (-1)^m
    return (seq_t)val;
}
