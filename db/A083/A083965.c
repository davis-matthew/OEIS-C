/*
 * A083965  a(n) = A083964(n)/(2n-1).
 * https://oeis.org/A083965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A083964(n)/(2n-1), where A083964(n) is the smallest
 *            palindrome k*X*rev(k) (k=2n-1, X a palindrome) divisible by k,
 *            or 0; found by search
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 121, u64 n <= 4090,
 *            u128 n <= 4090, big any n
 * Validated: standard, pass, 60 terms through n = 3906 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A083965_rev(uint64_t x) {
    uint64_t r = 0;
    while (x > 0) { r = r * 10 + x % 10; x /= 10; }
    return r;
}

static uint64_t A083965_gcd(uint64_t a, uint64_t b) {
    while (b != 0) { uint64_t t = b; b = a % b; a = t; }
    return a;
}

static bool A083965_find(uint64_t k, uint64_t revk, uint64_t p10c_mod, size_t max_m,
                         char *mid_out, size_t *len_out) {
    uint64_t revk_mod = revk % k;
    for (size_t m = 1; m <= max_m; ++m) {
        size_t half = (m + 1) / 2;
        uint64_t limit = 1;
        for (size_t i = 0; i < half; ++i) limit *= 10ULL;
        for (uint64_t y = 0; y < limit; ++y) {
            uint8_t hd[32];
            uint8_t dg[64];
            uint64_t tmp = y;
            for (size_t i = half; i-- > 0;) { hd[i] = (uint8_t)(tmp % 10); tmp /= 10; }
            for (size_t i = 0; i < half; ++i) dg[i] = hd[i];
            if (m % 2 == 0) {
                for (size_t i = 0; i < half; ++i) dg[half + i] = hd[half - 1 - i];
            } else {
                for (size_t i = 0; i + 1 < half; ++i) dg[half + i] = hd[half - 2 - i];
            }
            uint64_t xm = 0;
            for (size_t i = 0; i < m; ++i) xm = (xm * 10 + dg[i]) % k;
            if ((xm * p10c_mod + revk_mod) % k == 0) {
                for (size_t i = 0; i < m; ++i) mid_out[i] = (char)(dg[i] + '0');
                mid_out[m] = '\0';
                *len_out = m;
                return true;
            }
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    uint64_t k = 2ULL * (uint64_t)n - 1ULL;
    uint64_t revk = A083965_rev(k);
    if (revk == k) return (seq_t)1;

    uint64_t c = 0;
    { uint64_t t = k; while (t > 0) { ++c; t /= 10; } }

    uint64_t p10 = 1;
    for (uint64_t i = 0; i < c; ++i) p10 *= 10ULL;
    uint64_t g = A083965_gcd(k, p10);
    if (revk % g != 0) return (seq_t)0;

    uint64_t p10mod = 1 % k;
    for (uint64_t i = 0; i < c; ++i) p10mod = (p10mod * 10) % k;

    char mid[64];
    size_t mid_len = 0;
    if (!A083965_find(k, revk, p10mod, 12, mid, &mid_len)) return (seq_t)0;

    /* long division of the digit string k|mid|revk by k */
    char buf[128];
    size_t L = 0;
    {
        char tmp[24]; size_t tl = 0;
        uint64_t t = k;
        while (t > 0) { tmp[tl++] = (char)('0' + t % 10); t /= 10; }
        while (tl > 0) buf[L++] = tmp[--tl];
        for (size_t i = 0; i < mid_len; ++i) buf[L++] = mid[i];
        /* revk digits: reverse of k's digits (k has no trailing zero since odd) */
        t = k;
        while (t > 0) { buf[L++] = (char)('0' + t % 10); t /= 10; }
    }
    uint64_t rem = 0;
    seq_t q = 0;
    for (size_t i = 0; i < L; ++i) {
        rem = rem * 10 + (uint64_t)(buf[i] - '0');
        q = q * 10 + (seq_t)(rem / k);
        rem %= k;
    }
    return q;
}
