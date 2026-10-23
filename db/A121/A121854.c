/*
 * A121854  a(n) = floor(Pi*(sqrt(n))).
 * https://oeis.org/A121854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(pi * sqrt(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the rational approximation pi ≈ 104348/33215
   and integer arithmetic only. */

#include <stdint.h>

static inline unsigned __int128 isqrt128(unsigned __int128 x) {
    /* Computes floor(sqrt(x)) for 0 <= x < 2^128.
       Uses binary search on the range 0 .. 2^64-1. */
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    unsigned __int128 mid, sq;

    while (low <= high) {
        mid = (low + high) >> 1;
        sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;           /* floor sqrt */
}

seq_t a(idx_t n) {
    /* Constants for pi approximation: pi ≈ 104348 / 33215 */
    const unsigned __int128 P = (unsigned __int128)104348;
    const unsigned __int128 Q = (unsigned __int128)33215;

    unsigned __int128 n128 = (unsigned __int128)n;
    /* M = P^2 * n */
    unsigned __int128 M = P * P * n128;

    /* s = floor(P * sqrt(n)) = floor(sqrt(P^2 * n)) */
    unsigned __int128 s = isqrt128(M);

    /* a(n) = floor((P * sqrt(n)) / Q) */
    unsigned __int128 res = s / Q;

    return (seq_t)res;
}
