/*
 * A071089  Remainder when sum of first n primes is divided by n-th prime.
 * https://oeis.org/A071089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of first n primes mod nth prime a(n) = s[n] –
 *            p[n]*floor(s[n]/p[n])
 * Range:     u8 n <= 13, u16 n <= 781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n=0, but return 0 */
        return (seq_t)0;

    /* Static cache of primes and their prefix sums. */
    static uint64_t *primes   = NULL;   /* primes[0] = 2        */
    static seq_t   *prefix   = NULL;   /* prefix[i] = sum of primes[0..i] */
    static size_t  cached   = 0;        /* number of stored primes    */
    static uint64_t next_candidate = 3; /* next odd to test after 2   */

    /* Grow cache until it contains at least n primes. */
    while (cached < (size_t)n) {
        uint64_t candidate;

        if (cached == 0)
            candidate = 2;          /* first prime */
        else
            candidate = next_candidate;  /* next odd candidate */

        /* Test primality by trial division. */
        bool is_prime = true;
        for (size_t i = 0; i < cached; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        /* Update the candidate for the next iteration. */
        if (candidate == 2)
            next_candidate = 3;
        else
            next_candidate = candidate + 2;

        if (!is_prime)
            continue;    /* skip composite numbers */

        /* Append the new prime to the cache. */
        uint64_t *new_primes = (uint64_t *)realloc(primes,
                                         (cached + 1) * sizeof(uint64_t));
        seq_t   *new_prefix = (seq_t   *)realloc(prefix,
                                         (cached + 1) * sizeof(seq_t));
        if (!new_primes || !new_prefix) abort();  /* allocation failure */

        primes   = new_primes;
        prefix   = new_prefix;
        primes[cached] = candidate;
        if (cached == 0)
            prefix[cached] = (seq_t)candidate;
        else
            prefix[cached] = prefix[cached - 1] + (seq_t)candidate;
        ++cached;
    }

    /* Compute the desired term. */
    seq_t sum   = prefix[n - 1];
    seq_t prime = (seq_t)primes[n - 1];
    seq_t q     = sum / prime;          /* floor division */
    return sum - prime * q;             /* remainder */
}
