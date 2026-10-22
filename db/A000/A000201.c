/*
 * A000201  Lower Wythoff sequence (a Beatty sequence): a(n) = floor(n*phi),
 *          where phi = (1+sqrt(5))/2 = A001622.
 * https://oeis.org/A000201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n*phi) = floor((n + sqrt(5*n^2))/2) = (n +
 *            isqrt(5*n^2))/2
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a201_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a201_isqrt128.h"

seq_t a(idx_t n)
{
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 r = a201_isqrt128(5 * N * N);
    /* sqrt(5)*n is irrational for n>=1, so floor((n+sqrt(5n^2))/2) = floor((n+isqrt)/2) */
    uint64_t v = (uint64_t)(N + (r - N) / 2);
    return (seq_t)v;
}
