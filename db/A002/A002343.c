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
 * Helpers:   a2343_isprime, a2343_isqrt, a2343_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2343_isprime.h"
#include "../utils/a2343_isqrt.h"
#include "../utils/a2343_nth_prime.h"

seq_t a(idx_t n) {
    uint64_t p = a2343_nth_prime(n);
    uint64_t y;
    for (y = 1;; y++) {
        uint64_t v = 4 * p + 5 * y * y;
        uint64_t x = a2343_isqrt(v);
        if (x * x == v) return (seq_t)y;
    }
}
