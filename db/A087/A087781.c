/*
 * A087781  Number of non-congruent solutions to x^2 - x - 1 == 0 mod n.
 * https://oeis.org/A087781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = ∏_{p^k|n} a(p^k) where a(p^k) = { 1 if p = 5, k = 1 0 if
 *            p = 5, k > 1 0 if p ≡ 2 or 3 (mod 5) 2 if p ≡ 1 or 4 (mod 5) }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation relies on the utility function factor_u64
   from ../utils/factor_u64.h to factor n into its prime powers.
   The product is computed multiplicatively; if any factor yields 0,
   the whole product becomes 0.  For n = 1 the result is 1. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) = 1 by definition. */
    if (n == 1) return (seq_t)1;

    /* Factor n into prime powers p^k. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t k = exps[i];

        if (p == 5ULL) {
            /* a(5^k) = 1 if k = 1, otherwise 0. */
            if (k > 1ULL) return (seq_t)0;
            continue;                /* multiply by 1 */
        }

        uint64_t r = p % 5ULL;
        if (r == 2ULL || r == 3ULL) {
            /* a(p^k) = 0 for primes ≡ 2,3 (mod 5). */
            return (seq_t)0;
        }

        /* For p ≡ 1 or 4 (mod 5), a(p^k) = 2 for all k ≥ 1. */
        if (r == 1ULL || r == 4ULL) {
            /* Avoid intermediate overflow by using uint64_t before cast. */
            res = (seq_t)((uint64_t)res * 2ULL);
        }
    }

    return res;
}
