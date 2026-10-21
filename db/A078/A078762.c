/*
 * A078762  Numbers n such that n + sigma(n) is prime.
 * https://oeis.org/A078762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1608, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the integers n for which n + σ(n) is prime,
   where σ(n) is the sum of all divisors of n (including 1 and n). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: test whether a 64‑bit integer is prime.
   The implementation is provided by the ``utils/_is_prime.h`` header. */
#include "../utils/_is_prime.h"

/* Note: ``seq_t`` is an unsigned integer type; ``idx_t`` is an unsigned 64‑bit type. */

seq_t a(idx_t n)
{
    /* static state keeps the sequence and the list of primes up to the
       maximum number examined so far.  The state is retained between
       successive calls so that the sequence can be computed in any order
       without re‑generating earlier terms. */
    static seq_t *terms = NULL;        /* array of found terms */
    static size_t terms_len = 0;       /* number of terms stored */
    static size_t terms_cap = 0;       /* capacity of the terms array */

    static uint64_t *primes = NULL;    /* primes up to the current candidate */
    static size_t primes_len = 0;
    static size_t primes_cap = 0;

    static uint64_t last_checked = 0;  /* last integer examined for the property */

    /* generate terms until we have at least the n-th one   */
    while (terms_len < n)
    {
        /* candidate number to test */
        seq_t cand = ++last_checked;

        /* compute σ(cand) by prime factorisation using the
           accumulated list of primes.  This is efficient enough
           for the range needed by the tests described in the
           problem statement (up to about 10 000 terms).   */
        seq_t sigma = 1;
        seq_t tmp = cand;
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint64_t p = primes[i];
            if ((uint64_t)p * p > tmp) break;

            if (tmp % p == 0)
            {
                seq_t sum = 1;
                seq_t term = 1;
                while (tmp % p == 0)
                {
                    tmp /= p;
                    term *= p;          /* p^k   */
                    sum += term;        /* 1 + p + … + p^k  */
                }
                sigma *= sum;
            }
        }
        if (tmp > 1)      /* remaining prime factor */
            sigma *= (1 + tmp);

        /* if cand + σ(cand) is prime, store the candidate */
        seq_t cand_plus_sigma = cand + sigma;
        if (_is_prime((uint64_t)cand_plus_sigma))
        {
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_len++] = cand;
        }

        /* If the candidate itself is prime, add it to the prime list
           for future factorisations.  This keeps the prime list
           up‑to‑date without an explicit sieve.   */
        if (_is_prime((uint64_t)cand))
        {
            if (primes_len == primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                primes_cap = new_cap;
            }
            primes[primes_len++] = (uint64_t)cand;
        }
    }

    return terms[n - 1];
}
