/*
 * A053622  a(n) = round( n/log(n) - pi(n) ).
 * https://oeis.org/A053622
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = round( n/log(n) - pi(n) )
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A053622.  It works for any
   build where seq_t is an integer type and idx_t is uint64_t.
   It uses a simple sieve of Eratosthenes and caches pi(i)
   up to the largest n requested so far. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2.  For smaller n we
       return 0 – the behaviour is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* ------------------------------------------------------------------
       Static data kept between calls
       ------------------------------------------------------------------ */
    static idx_t      max_n = 1;            /* largest n processed so far */
    static uint8_t   *is_prime = NULL;      /* bitmap of primes          */
    static uint32_t  *pi_cache = NULL;      /* cumulative prime count    */
    static size_t     pi_len   = 0;          /* length of pi_cache        */

    /* ------------------------------------------------------------------
       If we are asked for a larger n, extend the sieve and pi_cache.
       ------------------------------------------------------------------ */
    if (n > max_n) {
        /* new size must include the index n itself */
        size_t newsize = (size_t)(n + 1);

        /* reallocate the prime bitmap */
        uint8_t *tmp_prime = (uint8_t *)realloc(is_prime, newsize);
        if (!tmp_prime)
            abort();
        is_prime = tmp_prime;

        /* initialise all entries to 1 (prime) */
        memset(is_prime, 1, newsize);
        is_prime[0] = 0;
        is_prime[1] = 0;

        /* simple Eratosthenes sieve up to n */
        idx_t limit = (idx_t)sqrt((double)n);
        for (idx_t p = 2; p <= limit; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= n; j += p)
                    is_prime[j] = 0;
            }
        }

        /* reallocate the pi_cache */
        size_t newpi = newsize;
        uint32_t *tmp_pi = (uint32_t *)realloc(pi_cache, newpi * sizeof(uint32_t));
        if (!tmp_pi)
            abort();
        pi_cache = tmp_pi;
        pi_len = newpi;

        /* compute cumulative prime count */
        pi_cache[0] = 0;
        for (size_t i = 1; i < newpi; ++i)
            pi_cache[i] = pi_cache[i - 1] + (is_prime[i] ? 1U : 0U);

        max_n = n;
    }

    /* ------------------------------------------------------------------
       Retrieve pi(n) from the cache
       ------------------------------------------------------------------ */
    uint32_t pi_n = pi_cache[n];

    /* ------------------------------------------------------------------
       Compute round( n/log(n) - pi(n) ) using floating‑point arithmetic.
       For n up to about 10^6 the double precision is more than enough
       to guarantee correct rounding to the nearest integer.
       ------------------------------------------------------------------ */
    double nd  = (double)n;
    double val = nd / log(nd) - (double)pi_n;
    long long rounded_ll = llround(val);          /* nearest integer */

    return (seq_t)rounded_ll;
}
