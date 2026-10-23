/*
 * A125072  a(n) = number of exponents in the prime-factorization of n which
 *          are triangular numbers.
 * https://oeis.org/A125072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of exponents in the prime‑factorization of n which
 *            are triangular numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Precomputed triangular numbers up to 64 – the maximum exponent for a 64‑bit integer. */
    static bool triangular[65] = {0};
    static bool initialized = false;
    if (!initialized) {
        for (uint64_t k = 1;; ++k) {
            uint64_t t = k * (k + 1) / 2;
            if (t > 64) break;
            triangular[(size_t)t] = true;
        }
        initialized = true;
    }

    /* Sequence has offset 1.  a(1) = 0.  For n < 1 we return 0 as well. */
    if (n < 1) return 0;

    /* Factorise n.  For 64‑bit n there are at most 12 distinct prime factors. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t count = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e <= 64 && triangular[(size_t)e]) {
            ++count;
        }
    }
    return count;
}
