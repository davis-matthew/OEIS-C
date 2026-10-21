/*
 * A060715  Number of primes between n and 2n exclusive.
 * https://oeis.org/A060715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(2n-1) - pi(n) (number of primes strictly between n and
 *            2n)
 * Range:     u8 n <= 2049, u16 n <= 9016, u32 n <= 9016, u64 n <= 9016,
 *            u128 n <= 9016, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache: store primes up to the largest 2n seen so far */
static idx_t cached_limit = 0;          /* current maximum value of 2n-1 computed */
static uint8_t *prime_cache = NULL;     /* 0 = composite, 1 = prime */
static seq_t *pi_cache    = NULL;       /* prefix sums: pi_cache[i] = number of primes <= i */

/* Helper: build sieve and prefix sums up to limit (inclusive) */
static void build_sieve(idx_t limit)
{
    /* free old caches if any */
    if (prime_cache) free(prime_cache);
    if (pi_cache)     free(pi_cache);

    prime_cache = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    pi_cache    = (seq_t   *)malloc((limit + 1) * sizeof(seq_t));
    if (!prime_cache || !pi_cache) {
        /* In an unlikely out‑of‑memory situation, abort */
        exit(1);
    }

    /* initialize: integers >=2 are tentatively prime */
    for (idx_t i = 0; i <= limit; ++i) prime_cache[i] = 1;
    if (limit >= 0) prime_cache[0] = 0;
    if (limit >= 1) prime_cache[1] = 0;

    /* sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime_cache[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                prime_cache[mult] = 0;
        }
    }

    /* build prefix sums */
    pi_cache[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi_cache[i] = pi_cache[i - 1] + (seq_t)prime_cache[i];

    cached_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */

    idx_t limit = 2 * n - 1;         /* we need primes < 2n, i.e. <= limit */

    if (limit > cached_limit) {
        build_sieve(limit);
    }
    /* If limit <= cached_limit, the cache already covers it */
    /* number of primes in (n, 2n) = pi(2n-1) - pi(n) */
    return pi_cache[limit] - pi_cache[n];
}
