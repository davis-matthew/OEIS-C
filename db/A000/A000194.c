/*
 * A000194  n appears 2n times, for n >= 1; also nearest integer to square
 *          root of n.
 * https://oeis.org/A000194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ceiling(obrt(n)) = least k >= 0 with k*(k+1) >= n, i.e.
 *            a(n) = floor((1 + sqrt(4*n - 3))/2) for n >= 1, a(0) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a194_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a194_isqrt.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    unsigned __int128 k = a194_isqrt(N);
    /* smallest k with k*(k+1) >= N */
    while (k * (k + 1) < (unsigned __int128)N) k++;
    while (k > 0 && (k - 1) * k >= (unsigned __int128)N) k--;
    return (seq_t)(uint64_t)k;
}
