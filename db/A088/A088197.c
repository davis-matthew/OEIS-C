/*
 * A088197  First differences of A088196.
 * https://oeis.org/A088197
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A088196(n+1) - A088196(n), where A088196(n) is the
 *            largest quadratic non-residue mod prime(n); computed by search
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 99 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A088197_isp(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A088197_nthprime(uint64_t n) {
    uint64_t c = 0, x = 1;
    while (c < n) {
        ++x;
        if (A088197_isp(x)) ++c;
    }
    return x;
}

static uint64_t A088197_pm(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    a %= m;
    while (e) {
        if (e & 1) r = (uint64_t)((unsigned __int128)r * a % m);
        a = (uint64_t)((unsigned __int128)a * a % m);
        e >>= 1;
    }
    return r;
}

static uint64_t A088197_lnr(uint64_t p) {
    if (p == 2) return 1;
    for (uint64_t k = p - 1; k > 0; --k)
        if (A088197_pm(k, (p - 1) / 2, p) == p - 1) return k;
    return 1;
}

seq_t a(idx_t n) {
    uint64_t p = A088197_nthprime((uint64_t)n);
    uint64_t q = p + 1;
    while (!A088197_isp(q)) ++q;
    seq_t x = (seq_t)A088197_lnr(q);
    seq_t y = (seq_t)A088197_lnr(p);
    return x - y;
}
