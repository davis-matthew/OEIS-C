/*
 * A082568  First nontrivial square root of unity mod A033949(n); i.e.,
 *          smallest x > 1 such that x^2 == 1 mod A033949(n).
 * https://oeis.org/A082568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x > 1 with x^2 == 1 (mod A033949(n)); found by
 *            search over A033949 candidates, then over x
 * Range:     u8 n <= 347, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 9997 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A082568_oddpp(uint64_t r) {
    uint64_t p = 0;
    uint64_t i;
    if (r <= 1) return false;
    for (i = 3; i * i <= r; i += 2) {
        if (r % i == 0) { p = i; break; }
    }
    if (p == 0) return true; /* r is an odd prime */
    while (r % p == 0) r /= p;
    return r == 1;
}

static bool A082568_noroot(uint64_t m) {
    unsigned e = 0;
    uint64_t r = m;
    while ((r & 1ULL) == 0) { r >>= 1; e++; }
    if (r == 1) return e >= 3;
    if (e >= 2) return true;
    return !A082568_oddpp(r);
}

seq_t a(idx_t n) {
    uint64_t target = (uint64_t)n;
    uint64_t count = 0;
    uint64_t m = 2;
    uint64_t x;
    while (count < target) {
        ++m;
        if (A082568_noroot(m)) ++count;
    }
    for (x = 2; x < m; ++x) {
        if ((x * x) % m == 1) break;
    }
    return (seq_t)x;
}
