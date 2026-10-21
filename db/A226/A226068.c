/*
 * A226068  The sum of the positive integers not exceeding 2n that are
 *          representable as the sum of two successive sexy primes.
 * https://oeis.org/A226068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Σ_{i=1..n} i · (prime(i-3) && prime(i+3))
 * Range:     u8 n <= 33, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a sieve up to n+3 and a cumulative cache. */

#include <stdlib.h>
#include <stdbool.h>

/* The function required by the build environment. */
seq_t a(idx_t n)
{
    /* Zero index for n < 1 is not defined; return 0 in that case. */
    if (n < 1) return (seq_t)0;

    /* --------------------------------------------------------------------
       Prime sieve: prime[x] is true iff x is prime, for 0 <= x < prime_cap.
       Build only as far as needed (n+3). */
    static bool *prime = NULL;
    static size_t  prime_cap = 0;      /* number of valid entries in prime[] */

    size_t required_prime_cap = (size_t)n + 4;   /* indices 0 .. n+3 inclusive */

    if (prime_cap < required_prime_cap) {
        bool *new_prime = (bool *)malloc(required_prime_cap * sizeof(bool));
        if (!new_prime) return (seq_t)0;  /* allocation failure */

        /* Initially assume all numbers are prime. */
        for (size_t i = 0; i < required_prime_cap; ++i) new_prime[i] = true;
        /* 0 and 1 are not prime. */
        if (required_prime_cap > 0) new_prime[0] = false;
        if (required_prime_cap > 1) new_prime[1] = false;

        /* Eratosthenes sieve up to required_prime_cap-1. */
        size_t limit = 0;
        while ((limit + 1) * (limit + 1) <= required_prime_cap - 1) ++limit;
        for (size_t p = 2; p <= limit; ++p) {
            if (new_prime[p]) {
                for (size_t m = p * p; m < required_prime_cap; m += p)
                    new_prime[m] = false;
            }
        }

        free(prime);
        prime      = new_prime;
        prime_cap = required_prime_cap;
    }

    /* --------------------------------------------------------------------
       Cumulative sums cache: cum[i] holds a(i) for 0 <= i <= cum_len-1. */
    static seq_t *cum = NULL;
    static size_t  cum_len = 0;          /* number of elements stored in cum[] */

    size_t new_len = (size_t)n + 1;   /* need indices 0 .. n */

    if (cum_len < new_len) {
        seq_t *tmp = (seq_t *)realloc(cum, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cum = tmp;

        /* If this is the first allocation, initialize a(0)=0. */
        if (cum_len == 0) {
            cum[0] = (seq_t)0;
            cum_len = 1;                 /* we now have a(0) computed */
        }

        /* Compute new cumulative entries from cum_len to new_len-1. */
        for (size_t i = cum_len; i < new_len; ++i) {
            seq_t delta = 0;
            if (i >= 3 && prime[i - 3] && prime[i + 3]) {
                /* i is the index for the squared sum 2*i. */
                delta = (seq_t)2 * (seq_t)i;
            }
            /* cum[i] = cum[i-1] + delta;  i>=1 always true here. */
            cum[i] = cum[i - 1] + delta;
        }
        cum_len = new_len;
    }

    /* Return the cached value. */
    return cum[(size_t)n];
}
