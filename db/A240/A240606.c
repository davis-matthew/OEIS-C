/*
 * A240606  Let the prime factorization of (2*n)! be 2^e_1*3^e_2*5^e_3*...;
 *          then a(n) = maximal k such that e_1, ..., e_k are all even.
 * https://oeis.org/A240606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal k such that the first k prime exponents in (2·n)!
 *            are all even
 * Range:     u8 n <= 2014, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t and idx_t provided by the build environment. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined below offset, return 0 as safe default */

    /* limit = 2·n, which is small enough for an in‑memory sieve (≤20 000 for n≤10 000). */
    idx_t limit = 2 * n;

    /* Sieve of Eratosthenes to enumerate primes up to limit. */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* Store primes in a dynamic array. */
    uint32_t *primes   = (uint32_t *)malloc(64 * sizeof(uint32_t));
    size_t    primes_cnt = 0, primes_cap = 64;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);

    /* Compute the maximal k. */
    seq_t k = 0;
    seq_t m = limit;               /* m = 2·n */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        seq_t expo = 0;
        seq_t t = m;
        while (t) {                 /* Legendre's formula */
            t /= p;
            expo += t;
        }
        if ((expo & 1U) == 0) {    /* even exponent */
            ++k;
        } else {                   /* first odd exponent stops the count */
            break;
        }
    }
    free(primes);
    return k;
}
