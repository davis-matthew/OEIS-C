/*
 * A100166  Structured deltoidal hexacontahedral numbers (vertex structure 9).
 * https://oeis.org/A100166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (112*n^3 - 156*n^2 + 50*n)/6
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 613, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    __uint128_t N = n;
    __uint128_t val = ( (__uint128_t)112 * N * N * N
                       - (__uint128_t)156 * N * N
                       + (__uint128_t)50 * N ) / 6;
    return (seq_t)val;
#else
    uint64_t N = n;
    uint64_t val = (112ULL * N * N * N - 156ULL * N * N + 50ULL * N) / 6ULL;
    return (seq_t)val;
#endif
}
