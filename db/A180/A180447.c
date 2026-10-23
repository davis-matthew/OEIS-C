/*
 * A180447  n appears 3n+1 times.
 * https://oeis.org/A180447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((sqrt(24*n+1)+1)/6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned __int128 values */
static unsigned long long isqrt128(unsigned __int128 x) {
    unsigned long long lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    unsigned __int128 val = (unsigned __int128)24 * n + 1;
    unsigned long long sqrt_val = isqrt128(val);
    seq_t res = (seq_t)((sqrt_val + 1) / 6);
    return res;
}
