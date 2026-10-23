/*
 * A125073  a(n) = sum of the exponents in the prime factorization of n which
 *          are triangular numbers.
 * https://oeis.org/A125073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the exponents in the prime factorization of n that
 *            are triangular numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static bool is_triangular(uint64_t e)
{
    static bool tri[65] = {0};
    static bool initialized = false;
    if (!initialized) {
        for (uint64_t k = 1; k <= 64; ++k) {
            uint64_t t = k * (k + 1) / 2;
            if (t <= 64) tri[(size_t)t] = true;
        }
        initialized = true;
    }
    return e <= 64 ? tri[(size_t)e] : false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1 */

    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exponents, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exponents[i];
        if (is_triangular(e)) {
            sum += (seq_t)e;
        }
    }
    return sum;
}
