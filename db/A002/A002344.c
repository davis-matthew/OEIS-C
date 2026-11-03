/*
 * A002344  Numbers x such that p = x^2 + 7y^2, with prime p = A033207(n).
 * https://oeis.org/A002344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: walk through the primes p in increasing order, keep
 *            those with p = x^2 + 7*y^2 (these are 7 and the primes p == 1,
 *            2, 4 mod 7); a(n) is the x >= 0 for the n-th such prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 899 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

/* returns 1 and sets *x if p = x^2 + 7 y^2 */
static int a2344_rep(uint64_t p, uint64_t *x) {
    uint64_t y;
    for (y = 1; 7 * y * y <= p; y++) {
        uint64_t r = p - 7 * y * y;
        uint64_t s = (uint64_t)__builtin_sqrt((double)r);
        while (s * s > r) s--;
        while ((s + 1) * (s + 1) <= r) s++;
        if (s * s == r) {
            *x = s;
            return 1;
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p;
    (void)isqrt_u64;
    for (p = 2;; p++) {
        uint64_t m = p % 7;
        uint64_t x;
        if (!(p == 7 || m == 1 || m == 2 || m == 4)) continue;
        if (!is_prime_u64(p)) continue;
        if (a2344_rep(p, &x)) {
            count++;
            if (count == n) return (seq_t)x;
        }
    }
}
