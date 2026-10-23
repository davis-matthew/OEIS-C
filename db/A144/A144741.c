/*
 * A144741  Number of semiprimes less than or equal to and not coprime to n.
 * https://oeis.org/A144741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of semiprimes ≤ n that share a prime factor with n
 *            * (i.e. are not coprime to n). * Implementation uses a simple
 *            sieve to generate primes up to n and * checks each semiprime p·q
 *            (p ≤ q) for a common factor with n
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   _has_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_has_factor.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* ------------------------------------------------------------------ */
    /* Step 1: generate the list of primes ≤ n using a trivial sieve.    */
    /* ------------------------------------------------------------------ */
    size_t limit = (size_t)n;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0;  /* allocation failure – return 0 */

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* collect primes into a dynamic array */
    size_t primes_cap = 8;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    primes_cap * sizeof(uint64_t));
                if (!tmp) {
                    free(is_prime);
                    free(primes);
                    return (seq_t)0;
                }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint64_t)i;
        }
    }
    free(is_prime);

    /* ------------------------------------------------------------------ */
    /* Step 2: factorise n to get its distinct prime divisors.             */
    /* ------------------------------------------------------------------ */
    idx_t tmp = n;
    size_t fac_cap = 8;
    uint64_t *factors = (uint64_t *)malloc(fac_cap * sizeof(uint64_t));
    if (!factors) {
        free(primes);
        return (seq_t)0;
    }
    size_t fac_cnt = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp)
            break;
        if (tmp % p == 0) {
            if (fac_cnt == fac_cap) {
                fac_cap *= 2;
                uint64_t *tmp2 = (uint64_t *)realloc(factors,
                                                     fac_cap * sizeof(uint64_t));
                if (!tmp2) {
                    free(primes);
                    free(factors);
                    return (seq_t)0;
                }
                factors = tmp2;
            }
            factors[fac_cnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1) {  /* remaining prime factor > sqrt(n) */
        if (fac_cnt == fac_cap) {
            fac_cap *= 2;
            uint64_t *tmp2 = (uint64_t *)realloc(factors,
                                                 fac_cap * sizeof(uint64_t));
            if (!tmp2) {
                free(primes);
                free(factors);
                return (seq_t)0;
            }
            factors = tmp2;
        }
        factors[fac_cnt++] = (uint64_t)tmp;
    }

    /* ------------------------------------------------------------------ */
    /* Step 3: count semiprimes ≤ n that share a prime factor with n.     */
    /* ------------------------------------------------------------------ */
    seq_t count = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t pi = primes[i];
        for (size_t j = i; j < primes_cnt; ++j) {
            uint64_t pj = primes[j];
            idx_t prod = (idx_t)pi * (idx_t)pj;
            if (prod > n)
                break;
            if (_has_factor(pi, factors, fac_cnt) ||
                _has_factor(pj, factors, fac_cnt)) {
                ++count;
            }
        }
    }

    /* ------------------------------------------------------------------ */
    /* Clean up                                                         */
    /* ------------------------------------------------------------------ */
    free(primes);
    free(factors);
    return count;
}
