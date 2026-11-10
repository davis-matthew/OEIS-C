/*
 * A092510  Difference between smallest semiperimeter (see A063655) and its
 *          integer log (see A001414).
 * https://oeis.org/A092510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A063655(n) - A001414(n), where A063655(n) = Min_{d|n}
 *            (n/d + d) and A001414(n) = sum of prime factors of n with
 *            repetition
 * Range:     u8 n <= 6136, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 7047 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A063655_a(uint64_t n)
{
    uint64_t best = n + 1;
    for (uint64_t d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            best = d + n / d;
        }
    }
    return best;
}

static uint64_t A001414_a(uint64_t n)
{
    if (n == 1) return 0;
    uint64_t result = 0;
    uint64_t temp = n;
    while (temp % 2 == 0) {
        temp /= 2;
        result += 2;
    }
    for (uint64_t p = 3; temp / p >= p; p += 2) {
        while (temp % p == 0) {
            temp /= p;
            result += p;
        }
    }
    if (temp > 1) result += temp;
    return result;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    return (seq_t)(A063655_a(m) - A001414_a(m));
}
