/*
 * A089801  a(n) = 0 unless n = 3j^2 + 2j or 3j^2 + 4j + 1 for some j >= 0, in
 *          which case a(n) = 1.
 * https://oeis.org/A089801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 3j^2+2j or n = 3j^2+4j+1 for some j ≥ 0, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a non‑negative 128‑bit integer, result fits in 64 bits */
static uint64_t isqrt_u128(__int128 x) {
    if (x <= 0) return 0;
    uint64_t low = 0;
    uint64_t high = 1;
    /* grow high until high^2 > x */
    while ((__int128)high * high <= x) {
        high <<= 1;
    }
    /* binary search between low and high */
    while (low + 1 < high) {
        uint64_t mid = (low + high) >> 1;
        if ((__int128)mid * mid <= x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    if ((__int128)high * high <= x) return high;
    return low;
}

seq_t a(idx_t n) {
    /* compute D = 4 + 12*n */
    __int128 D = (__int128)12 * n + 4;
    uint64_t s = isqrt_u128(D);
    if ((__int128)s * s != D) return (seq_t)0;   /* D must be a perfect square */

    /* first representation: n = 3j^2+2j  ->  j = (s-2)/6  */
    if (s >= 2 && (s - 2) % 6 == 0) return (seq_t)1;

    /* second representation: n = 3j^2+4j+1 -> j = (s-4)/6 */
    if (s >= 4 && (s - 4) % 6 == 0) return (seq_t)1;

    return (seq_t)0;
}
