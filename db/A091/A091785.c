/*
 * A091785  Evil numbers (see A001969) in A003159.
 * https://oeis.org/A091785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A003159(2*n), where A003159(k) = smallest m with
 *            Sum_{i>=0} floor((floor(m/4^i)+1)/2) >= k (binary search)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9975 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a091785_count(uint64_t m) {
    uint64_t c = 0;
    while (m > 0) {
        c += (m + 1) / 2;
        m >>= 2;
    }
    return c;
}

seq_t a(idx_t n) {
    uint64_t k = 2 * (uint64_t)n;
    uint64_t lo = k, hi = 2 * k;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a091785_count(mid) >= k)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
