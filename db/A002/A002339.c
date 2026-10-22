/*
 * A002339  Positive y such that p = (x^2 + 27*y^2)/4 where p is the n-th
 *          prime of the form 6*k+1.
 * https://oeis.org/A002339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: p = n-th prime of the form 6k+1 (A002476); a(n) = the
 *            unique y > 0 with 4*p = x^2 + 27*y^2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a2339_isprime, a2339_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2339_isprime.h"
#include "../utils/a2339_isqrt.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 1;
    uint64_t target, y;

    while (count < n) {
        p += 6;
        if (a2339_isprime(p)) count++;
    }

    target = 4 * p;
    for (y = 1; 27 * y * y < target; y++) {
        uint64_t xs = target - 27 * y * y;
        uint64_t x = a2339_isqrt(xs);
        if (x * x == xs && x % 3 != 0) return (seq_t)y;
    }
    return (seq_t)0;
}
