/*
 * A002365  Numbers y such that p^2 = x^2 + y^2, 0 < x < y, p = A002144(n).
 * https://oeis.org/A002365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(b^2 - a^2, 2*a*b), where p = A002144(n) = a^2 + b^2;
 *            found by searching for the n-th prime == 1 (mod 4), then for its
 *            two-squares decomposition
 * Range:     u8 n <= 25, u16 n <= 3281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   s2365_isprime, s2365_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/s2365_isprime.h"
#include "../utils/s2365_isqrt.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 1;
    while (count < n) {
        p += 4;
        if (s2365_isprime(p)) count++;
    }
    uint64_t av = 0, bv = 0;
    for (uint64_t x = 1; 2 * x * x < p; x++) {
        uint64_t r = p - x * x;
        uint64_t y = s2365_isqrt(r);
        if (y * y == r) { av = x; bv = y; break; }
    }
    uint64_t c = bv * bv - av * av;
    uint64_t d = 2 * av * bv;
    seq_t res = (seq_t)(c > d ? c : d);
    return res;
}
