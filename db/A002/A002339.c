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
 * Validated: standard, pass, 134 terms through n = 942 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 1;
    uint64_t target, y;

    while (count < n) {
        p += 6;
        if (is_prime_u64(p)) count++;
    }

    target = 4 * p;
    for (y = 1; 27 * y * y < target; y++) {
        uint64_t xs = target - 27 * y * y;
        uint64_t x = isqrt_u64(xs);
        if (x * x == xs && x % 3 != 0) return (seq_t)y;
    }
    return (seq_t)0;
}
