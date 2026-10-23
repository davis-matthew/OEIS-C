/*
 * A124795  Number of permutations with given cycle structure, in the prime
 *          factorization order.
 * https://oeis.org/A124795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k1+2*k2+...)! / ((k1!*k2!*...)*(1^k1*2^k2*...))
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 78, u128 n <= 150,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but safe */

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    idx_t S = 0;          /* sum i*k_i */
    seq_t denom = 1;      /* denominator product */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute prime index (1-based) */
        size_t idx = 0;
        for (uint64_t q = 2; q <= p; ++q) {
            if (is_prime(q)) ++idx;
        } /* idx equals the position of p among primes starting with 2 */

        S += (idx_t)(idx) * (idx_t)e;

        /* compute i^e */
        seq_t pow = 1;
        for (uint64_t j = 0; j < e; ++j) pow *= (seq_t)(idx);

        /* compute e! */
        seq_t fac = 1;
        for (uint64_t j = 2; j <= e; ++j) fac *= (seq_t)j;

        denom *= pow;
        denom *= fac;
    }

    /* numerator: factorial of S */
    seq_t numer = 1;
    for (idx_t i = 2; i <= S; ++i) numer *= (seq_t)i;

    return numer / denom;
}
