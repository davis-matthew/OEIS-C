/*
 * A195150  Number of divisors d of n such that d-1 does not divide n.
 * https://oeis.org/A195150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that d-1 does not divide n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* a(1) = 0, a(0) undefined but return 0 */
    seq_t count = 0;
    idx_t sq = isqrt_u64(n);
    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;
        /* process d1 */
        if (d1 >= 2) {
            if (n % (d1 - 1) != 0) ++count;
        }
        /* process d2 if distinct from d1 */
        if (d2 != d1 && d2 >= 2) {
            if (n % (d2 - 1) != 0) ++count;
        }
    }
    return count;
}
