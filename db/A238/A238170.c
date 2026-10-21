/*
 * A238170  Integer part of square root of A001017: a(n) = floor(n^(9/2)).
 * https://oeis.org/A238170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(9/2)) = floor(sqrt(n^9))
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 138, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

static seq_t isqrt128(unsigned __int128 x) {
    unsigned __int128 low = 0, high = x, ans = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == x) return (seq_t)mid;
        if (sq < x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)ans;
}

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    /* If seq_t is a fixed‑size unsigned integer (≤ 64 bits),
       we compute n^9 in a 128‑bit integer to avoid overflow,
       then take the integer square root of that 128‑bit value.   */
#if defined(__SIZEOF_INT128__) || defined(_MSC_VER)
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 pow128 = 1;
        unsigned __int128 un = n;
        for (idx_t i = 0; i < 9; ++i) pow128 *= un;
        return isqrt128(pow128);
    }
#endif
    /* For arbitrary‑precision seq_t we use the supplied utilities.  */
    seq_t exp9 = 9;
    seq_t n9 = big_pow(sn, exp9);
    return floor_sqrt(n9);
}
