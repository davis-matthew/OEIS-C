/*
 * A001597  Perfect powers: m^k where m > 0 and k >= 2.
 * https://oeis.org/A001597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = smallest x with N(x) >= n, where N(x) = 1 - Sum_{k>=2}
 *            mu(k)*(floor(x^(1/k)) - 1) counts the perfect powers <= x; found
 *            by binary search on x (a(n) <= n^2)
 * Range:     u8 n <= 22, u16 n <= 298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001597_count, a001597_iroot, a001597_mu, a001597_pow_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a001597_count.h"
#include "../utils/a001597_iroot.h"
#include "../utils/a001597_mu.h"
#include "../utils/a001597_pow_le.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t lo = 1, hi = (uint64_t)n * (uint64_t)n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a001597_count(mid) >= (uint64_t)n) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)lo;
}
