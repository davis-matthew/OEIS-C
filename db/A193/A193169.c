/*
 * A193169  Number of odd divisors of lambda(n).
 * https://oeis.org/A193169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors of λ(n) (Carmichael function)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd_uint64.h"

/* Euclidean GCD for uint64_t */

seq_t a(idx_t n) {
    /* factor n into prime powers */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* compute λ(n) as lcm of λ(p^e). */
    uint64_t lambda = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        uint64_t term;

        if (p == 2ULL) {
            if (e == 1ULL)
                term = 1ULL;
            else if (e == 2ULL)
                term = 2ULL;
            else
                term = 1ULL << (e - 2ULL);      /* 2^(e-2) for e>=3 */
        } else {
            /* λ(p^e) = (p-1) * p^(e-1) for odd p */
            uint64_t powp = 1ULL;
            for (uint64_t k = 1ULL; k < e; ++k)
                powp *= p;
            term = (p - 1ULL) * powp;
        }

        uint64_t g = gcd_uint64(lambda, term);
        lambda = (lambda / g) * term;         /* lcm */
    }

    /* strip all factors of 2 from λ(n) */
    uint64_t odd = lambda;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1;

    /* factor the odd part and count its odd divisors */
    uint64_t pr2[64], ex2[64];
    size_t cnt2 = 0;
    factor_u64(odd, pr2, ex2, &cnt2);

    seq_t result = 1;
    for (size_t i = 0; i < cnt2; ++i)
        result *= (seq_t)(ex2[i] + 1);        /* (exponent + 1) */

    return result; /* 1 if odd == 1 (no odd primes) */
}
