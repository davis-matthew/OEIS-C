/*
 * A072014  Minima when the mapping of A072010 is applied to n repeatedly.
 * https://oeis.org/A072014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum of the trajectory n, f(n), f(f(n)), ... where f =
 *            A072010 (completely multiplicative: 2->2, p -> p+2 if p==1 mod
 *            4, p -> p-2 if p==3 mod 4). Computed by iterating f with Brent
 *            cycle detection and tracking the minimum
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A072014_f(uint64_t m) {
    uint64_t result = 1;
    while (m % 2 == 0) {
        m /= 2;
        result *= 2;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t t = (p % 4 == 1) ? p + 2 : p - 2;
            while (m % p == 0) {
                m /= p;
                result *= t;
            }
        }
    }
    if (m > 1) {
        uint64_t t = (m % 4 == 1) ? m + 2 : m - 2;
        result *= t;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n;
    uint64_t tortoise = x;
    uint64_t mn = x;
    uint64_t power = 1, lam = 0;
    uint64_t hare = x;
    for (;;) {
        hare = A072014_f(hare);
        if (hare < mn) mn = hare;
        if (hare == tortoise) break;
        ++lam;
        if (lam == power) {
            tortoise = hare;
            power *= 2;
            lam = 0;
        }
    }
    return (seq_t)mn;
}
