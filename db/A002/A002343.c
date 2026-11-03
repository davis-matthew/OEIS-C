/*
 * A002343  Least positive integer y such that p = (x^2 - 5*y^2)/4 where p is
 *          the n-th odd prime such that 5 is a square mod p.
 * https://oeis.org/A002343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: p = A038872(n) (p = 5 or prime p == +-1 mod 10); a(n) =
 *            least y >= 1 with 4*p + 5*y^2 a perfect square x^2
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 97 terms through n = 97 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

static uint64_t a2343_nth_prime(idx_t n) {
    idx_t count = 0;
    uint64_t p;
    if (n == 1) return 5;
    count = 1;
    for (p = 11;; p += 2) {
        uint64_t r = p % 10;
        if ((r == 1 || r == 9) && is_prime_u64(p)) {
            count++;
            if (count == n) return p;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t p = a2343_nth_prime(n);
    uint64_t y;
    for (y = 1;; y++) {
        uint64_t v = 4 * p + 5 * y * y;
        uint64_t x = isqrt_u64(v);
        if (x * x == v) return (seq_t)y;
    }
}
