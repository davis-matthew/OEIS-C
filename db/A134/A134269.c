/*
 * A134269  Number of solutions to the equation p^k - p^(k-1) = n, where k is
 *          a positive integer and p is prime.
 * https://oeis.org/A134269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to p^k - p^(k-1) = n (= number of
 *            primes p such that p-1 divides n and n/(p-1) is a power of p)
 * Range:     u8 n <= 7185, u16 n <= 9860, u32 n <= 9860, u64 n <= 9860,
 *            u128 n <= 9860, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cnt   = 0;          /* number of primes in cache */
static idx_t  prime_limit = 0;          /* maximum prime generated (inclusive) */

/* Simple sieve of Eratosthenes that fills the global cache up to a given limit.
   It frees the old cache before allocating a new one. */
static void build_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* free previous cache */
    if (prime_cache != NULL) free(prime_cache);
    /* small limit optimization */
    if (limit < 2) {
        prime_cache = NULL;
        prime_cnt   = 0;
        prime_limit = limit;
        return;
    }

    /* sieve array: 0 = prime, 1 = composite */
    char *sieve = (char *)calloc(limit + 1, sizeof(char));
    if (sieve == NULL) {
        /* allocation failure – in this teaching scenario we abort */
        exit(1);
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            ++cnt;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }

    prime_cache = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (prime_cache == NULL) {
        free(sieve);
        exit(1);
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            prime_cache[idx++] = (uint64_t)i;
        }
    }

    free(sieve);
    prime_cnt   = cnt;
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */

    /* Ensure we have all primes up to n+1 in the cache */
    build_primes_upto(n + 1);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        idx_t    d = (idx_t)(p - 1);       /* divisor candidate */

        if (n % d != 0) continue;         /* p-1 must divide n */

        seq_t m = (seq_t)(n / d);          /* m = n/(p-1) */
        seq_t pp = (seq_t)p;               /* prime as seq_t */

        /* Check whether m is a pure power of p */
        while (m > 1 && m % pp == 0) {
            m /= pp;
        }
        if (m == 1) {
            ++count;                       /* one valid solution found */
        }
    }

    return count;
}
