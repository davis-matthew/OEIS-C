/*
 * A202305  a(n) = floor(sqrt(5*n)).
 * https://oeis.org/A202305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(5*n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

static unsigned long long isqrt_u128(unsigned __int128 x) {
    unsigned long long lo = 0;
    unsigned long long hi = UINT64_MAX;
    while (hi >= lo) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi; /* floor sqrt */
}

seq_t a(idx_t n) {
    unsigned __int128 prod = (unsigned __int128)n * 5;
    unsigned long long root = isqrt_u128(prod);
    return (seq_t)root;
}
