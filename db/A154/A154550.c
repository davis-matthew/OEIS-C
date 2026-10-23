/*
 * A154550  Greater of two consecutive primes, p < q, such that p*q+p-q is
 *          prime.
 * https://oeis.org/A154550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th larger prime q of consecutive primes (p,q) such
 *            that p*q + p - q is prime. The function searches successive
 *            pairs of prime numbers (p,q) in increasing order, tests the
 *            expression, and records q whenever the expression is prime. The
 *            n‑th stored q is returned
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 12, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, return 0 */

    static uint64_t last_prime = 2;        /* last prime processed */

#ifndef __cplusplus
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
#else
    static std::vector<seq_t> cache;
#endif

    /* Generate terms until we have at least n of them */
    while (1)
    {
#ifndef __cplusplus
        if (cache_len >= (size_t)n) break;
#else
        if (cache.size() >= (size_t)n) break;
#endif
        uint64_t p = last_prime;
        uint64_t q = next_prime(p);      /* next prime after p */
        last_prime = q;                  /* advance for next iteration */

        seq_t expr = (seq_t)p * (seq_t)q + (seq_t)p - (seq_t)q;

        if (is_prime_big(expr))           /* condition satisfied */
        {
#ifndef __cplusplus
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)q;
#else
            cache.push_back((seq_t)q);
#endif
        }
    }

    /* Return the nth term (1‑based) */
#ifndef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
