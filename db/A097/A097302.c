/*
 * A097302  Denominators of rationals used in the Euler-Maclaurin type
 *          derivation of Stirling's formula for N!.
 * https://oeis.org/A097302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator( Bernoulli(2*n+2) * (2*n)!/(2*n+2) )
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/vp.h"

/* exponent of prime p in m! */
static idx_t fact_exp(idx_t m, uint32_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* m = 2*n + 2   (Bernoulli index) */
    idx_t m = 2 * n + 2;
    /* sieve limit = m + 1 to cover primes p with p-1 | m */
    idx_t limit = m + 1;

    /* static cache of primes */
    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_max = 0;

    if (prime_max < limit) {
        /* recompute primes up to new limit */
        size_t sz = (size_t)(limit + 1);
        char *is_prime = (char *)calloc(sz, 1);
        if (!is_prime) return 0;                         /* allocation failure */

        for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        for (size_t i = 2; i * i <= (size_t)limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= (size_t)limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* count primes and store them */
        size_t cnt = 0;
        for (size_t i = 2; i <= (size_t)limit; ++i)
            if (is_prime[i]) ++cnt;

        uint32_t *new_arr = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
        if (!new_arr) { free(is_prime); return 0; }
        primes = new_arr;
        prime_cnt = cnt;

        size_t pos = 0;
        for (size_t i = 2; i <= (size_t)limit; ++i)
            if (is_prime[i]) primes[pos++] = (uint32_t)i;

        prime_max = limit;
        free(is_prime);
    }

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (p > limit) break;

        idx_t exp_den = 0;
        /* von Staudt–Clausen part: prime p appears if (p-1) divides m */
        if (m % (p - 1) == 0)
            ++exp_den;
        /* exponent of p in m itself */
        exp_den += vp(m, p);
        /* exponent of p in (m-2)! */
        idx_t exp_fact = 0;
        if (m > 2)
            exp_fact = fact_exp(m - 2, p);

        idx_t e = (exp_den > exp_fact) ? (exp_den - exp_fact) : 0;
        for (idx_t k = 0; k < e; ++k)
            result *= (seq_t)p;
    }

    return result;
}
