/*
 * A003159  Numbers whose binary representation ends in an even number of
 *          zeros.
 * https://oeis.org/A003159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, eigen, base
 * Formula:   a(n) = smallest m with #{k <= m : k = 4^i*(2j+1)} >= n, where
 *            that count is Sum_{i>=0} floor((floor(m/4^i)+1)/2); found by
 *            binary search
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a003159_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003159_count.h"

seq_t a(idx_t n) {
    uint64_t lo = n, hi = 2 * (uint64_t)n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a003159_count(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
