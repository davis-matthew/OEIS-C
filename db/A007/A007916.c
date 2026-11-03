/*
 * A007916  Numbers that are not perfect powers.
 * https://oeis.org/A007916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least x with x - 1 - P(x) = n, where P(x) = -Sum_{k>=2}
 *            mu(k)*(floor(x^(1/k)) - 1) counts perfect powers in [2,x]; found
 *            by fixed-point iteration x <- n + 1 + P(x)
 * Range:     u8 n <= 233, u16 n <= 9875, u32 n <= 9875, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Validated: standard, pass, 87 terms through n = 9821 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* true if r^k <= x, without overflow */
static bool pow_le(uint64_t r, int k, uint64_t x) {
    uint64_t p = 1;
    int i;
    for (i = 0; i < k; i++) {
        if (r != 0 && p > x / r) return false;
        p *= r;
    }
    return p <= x;
}

static uint64_t iroot(uint64_t x, int k) {
    uint64_t lo = 1, hi;
    if (x == 0) return 0;
    if (k >= 64) return 1;
    hi = ((uint64_t)1 << (64 / k + 1 > 63 ? 63 : 64 / k + 1));
    if (hi > x) hi = x;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (pow_le(mid, k, x)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static int mobius_small(int k) {
    int m = 1, p;
    for (p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            k /= p;
            if (k % p == 0) return 0;
            m = -m;
        }
    }
    if (k > 1) m = -m;
    return m;
}

static uint64_t perfect_powers_upto(uint64_t x) {
    int64_t s = 0;
    int k;
    for (k = 2; k < 64; k++) {
        uint64_t r;
        int mu;
        if (((uint64_t)1 << k) > x) break;
        mu = mobius_small(k);
        if (mu == 0) continue;
        r = iroot(x, k);
        s -= (int64_t)mu * (int64_t)(r - 1);
    }
    return (uint64_t)s;
}

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n + 1;
    for (;;) {
        uint64_t nx = (uint64_t)n + 1 + perfect_powers_upto(x);
        if (nx == x) break;
        x = nx;
    }
    return (seq_t)x;
}
