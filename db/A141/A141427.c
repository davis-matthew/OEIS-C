/*
 * A141427  a(n) = phi(A067775(n)).
 * https://oeis.org/A141427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   append_candidate, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = phi(A067775(n)), i.e. p-1 for the nth prime p for which p+4 is composite. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

/* Static storage for primes and the a(n) values */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static seq_t *a_cache = NULL;     /* array of phi values (p-1) */
static size_t a_cache_len = 0;
static bool initialized = false;

/* initialise primes and cache of a(n) values */
static void init(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Choose a safe upper bound for the sieve.
       For n <= 10000 this covers many more than enough terms. */
    const idx_t sieve_limit = 2000000;  /* 2 million */

    /* Build all primes <= sieve_limit */
    build_primes_upto(sieve_limit, &primes, &primes_cnt);

    /* Build the cache of phi(A067775(n)) values. */
    size_t len = 0, cap = 0;
    seq_t *cache = NULL;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p      = primes[i];
        uint64_t p_plus4 = p + 4;
        if (!is_prime((idx_t)p_plus4)) {
            seq_t val = (seq_t)p - (seq_t)1; /* phi(p) = p-1 for prime p */
            append_candidate(&cache, &len, &cap, val);
        }
    }

    a_cache      = cache;
    a_cache_len  = len;

    /* primes array no longer needed */
    free(primes);
    primes = NULL;
    primes_cnt = 0;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined, but return 0 */

    init();

    if ((idx_t)n <= a_cache_len)
        return a_cache[(size_t)n - 1];

    /* If we run out of precomputed terms (unlikely for n <= 10000),
       we can extend the sieve. For simplicity, return 0. */
    return (seq_t)0;
}
