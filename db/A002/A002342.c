/*
 * A002342  Least positive integer x such that p = (x^2 - 5*y^2)/4 where p is
 *          the n-th odd prime such that 5 is a square mod p.
 * https://oeis.org/A002342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x > 0 with x^2 - 5*y^2 = 4*p, p = A038872(n); found
 *            by search: enumerate odd primes p = 5 or p == +-1 (mod 5), then
 *            smallest y >= 0 making 4p + 5y^2 a perfect square
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   a2342_isprime, a2342_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2342_isprime.h"
#include "../utils/a2342_isqrt.h"

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t p = 3;
    for (;;) {
        uint64_t r = p % 5;
        if ((r == 0 || r == 1 || r == 4) && a2342_isprime(p)) {
            count++;
            if (count == (uint64_t)n) break;
        }
        p += 2;
    }
    for (uint64_t y = 0;; y++) {
        uint64_t t = 4 * p + 5 * y * y;
        uint64_t x = a2342_isqrt(t);
        if (x * x == t) return (seq_t)x;
    }
}
