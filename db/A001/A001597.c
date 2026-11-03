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
 * Validated: standard, pass, 70 terms through n = 9984 (2026-11-01)
 * Helpers:   mobius_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius_i32.h"

static bool a001597_pow_le(uint64_t b, int k, uint64_t x) {
    uint64_t r = 1;
    for (int i = 0; i < k; i++) {
        if (r > x / b) return false;
        r *= b;
    }
    return true;
}

static uint64_t a001597_iroot(uint64_t x, int k) {
    int sh = 64 / k + 1;
    uint64_t lo = 1, hi = (sh >= 63) ? ((uint64_t)1 << 33) : ((uint64_t)1 << sh);
    if (x == 0) return 0;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (a001597_pow_le(mid, k, x)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static uint64_t a001597_count(uint64_t x) {
    long long c = 1;
    for (int k = 2; k < 64; k++) {
        uint64_t r = a001597_iroot(x, k);
        if (r < 2) break;
        c -= (long long)mobius_i32(k) * (long long)(r - 1);
    }
    return (uint64_t)c;
}

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
