/*
 * A065134  Remainder when n is divided by the number of primes not exceeding
 *          n.
 * https://oeis.org/A065134
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n mod pi(n) where pi(n) is the number of primes ≤ n
 * Range:     u8 n <= 285, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the n-th term of OEIS sequence A065134.
   For n < 2 the value is undefined; we simply return 0. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Static state: a simple incremental sieve */
    static size_t sieve_limit = 1;   /* maximum index currently sieved (inclusive) */
    static bool *is_prime = NULL;    /* sieve array, is_prime[i] == true iff i is prime */

    /* Grow the sieve if needed */
    if ((size_t)n > sieve_limit) {
        size_t new_limit = (sieve_limit + 1) * 2;
        if (new_limit < (size_t)n) new_limit = n;

        /* Allocate (or reallocate) the sieve array */
        bool *tmp = (bool *)realloc(is_prime, (new_limit + 1) * sizeof(bool));
        if (!tmp) abort();          /* allocation failed */
        is_prime = tmp;

        /* Initialise new entries as prime */
        for (size_t i = sieve_limit + 1; i <= new_limit; ++i)
            is_prime[i] = true;

        /* 0 and 1 are not prime */
        if (new_limit >= 0) is_prime[0] = false;
        if (new_limit >= 1) is_prime[1] = false;

        /* Run the sieve up to new_limit */
        for (size_t p = 2; p * p <= new_limit; ++p) {
            if (is_prime[p]) {
                for (size_t j = p * p; j <= new_limit; j += p)
                    is_prime[j] = false;
            }
        }

        sieve_limit = new_limit;
    }

    /* Count primes up to and including n */
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (is_prime[i]) ++prime_cnt;
    }

    /* Return the remainder of n divided by pi(n) */
    seq_t result = ((seq_t)n) % (seq_t)prime_cnt;
    return result;
}
