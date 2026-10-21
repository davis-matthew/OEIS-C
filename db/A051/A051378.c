/*
 * A051378  Sum of (1+e)-divisors of n. Let n = Product_i p(i)^r(i) then
 *          (1+e)-sigma(n) = Product_i (1 + Sum_{s|r(i)} p(i)^s).
 * https://oeis.org/A051378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) is multiplicative with a(p^e) = 1 + sum_{d|e} p^d
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* sequence starts at offset 1 */

    /* factorize n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t sum   = 0;
        seq_t p_pow = 1;

        /* sum p^d over all divisors d of e */
        for (uint64_t d = 1; d <= e; ++d) {
            p_pow *= (seq_t)p;
            if (e % d == 0) {
                sum += p_pow;
            }
        }

        result *= (1 + sum);
    }

    return result;
}
