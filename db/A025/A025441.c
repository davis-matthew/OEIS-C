/*
 * A025441  Number of partitions of n into 2 distinct nonzero squares.
 * https://oeis.org/A025441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (x,y) with 0 < y < x, x^2 + y^2
 *            = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t low = 1, high = (idx_t)1 << ((sizeof(idx_t)*8)/2);
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n < 5) return (seq_t)0;  /* first possible sum of two distinct squares is 5 (2^2+1^2) */

    seq_t count = 0;
    idx_t max_y = isqrt_u64(n / 2);
    for (idx_t y = 1; y <= max_y; ++y) {
        idx_t rem = n - y * y;
        idx_t x = isqrt_u64(rem);
        if (x * x == rem && x > y)
            ++count;
    }
    return count;
}
