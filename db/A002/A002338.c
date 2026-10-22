/*
 * A002338  x such that p = (x^2 + 27*y^2)/4, where p is the n-th prime of the
 *          form 3*k+1.
 * https://oeis.org/A002338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x where 4*p = x^2 + 27*y^2, p = n-th prime == 1 (mod 3);
 *            found by searching primes, then searching y
 * Range:     u8 n <= 939, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a002338_isprime, a002338_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a002338_isprime.h"
#include "../utils/a002338_isqrt.h"

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t p = 7;
    uint64_t four_p, y;

    for (;;) {
        if (a002338_isprime(p)) {
            count++;
            if (count == (uint64_t)n) break;
        }
        p += 6;
    }

    four_p = 4 * p;
    for (y = 1; 27 * y * y < four_p; y++) {
        uint64_t rem = four_p - 27 * y * y;
        uint64_t x = a002338_isqrt(rem);
        if (x * x == rem) return (seq_t)x;
    }
    return (seq_t)0;
}
