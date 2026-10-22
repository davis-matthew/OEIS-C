/*
 * A002341  Numbers y such that p = x^2 - 5y^2, where p = 0, 1, or 4 (mod 5).
 * https://oeis.org/A002341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: for the n-th prime p with p == 0, 1 or 4 (mod 5), a(n)
 *            is the least y >= 1 such that p + 5*y^2 is a perfect square x^2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   a2341_isprime, a2341_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2341_isprime.h"
#include "../utils/a2341_isqrt.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p;
    for (p = 2; ; p++) {
        uint64_t m5 = p % 5;
        uint64_t y;
        if (m5 != 0 && m5 != 1 && m5 != 4) continue;
        if (!a2341_isprime(p)) continue;
        count++;
        if (count < n) continue;
        for (y = 1; ; y++) {
            uint64_t s = p + 5 * y * y;
            uint64_t x = a2341_isqrt(s);
            if (x * x == s) return (seq_t)y;
        }
    }
}
