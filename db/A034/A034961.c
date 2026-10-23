/*
 * A034961  Sums of three consecutive primes.
 * https://oeis.org/A034961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + prime(n+1) + prime(n+2) (OEIS A034961)
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache of primes.  The primes are stored 1‑based in OEIS order:
 * prime_cache[0] = 2, prime_cache[1] = 3, ...   */
static uint64_t *prime_cache = NULL;
static size_t     prime_cnt   = 0;   /* current number of stored primes   */
static size_t     prime_limit = 0;   /* sieve limit for the current cache */



/* ------------------------------------------------------------------ */
/*  Generate all primes up to 'limit' (inclusive).  The result is    */
/*  put into prime_cache and prime_cnt.  This function overwrites any*/
/*  previous cache.  It is intentionally simple and recomputes the   */
/*  sieve from scratch each time it is called.                       */
static void generate_primes_up_to(size_t limit)
{
    /* Allocate a sieve array.  We use the smallest type that can hold 0/1. */
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) return;   /* In a production build you should handle OOM. */

    /* Initialise all entries to true (1). */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Classic Eratosthenes sieve */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                is_prime[q] = 0;
        }
    }

    /* Count primes and copy them into the cache. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    /* Reallocate cache to fit exactly the counted primes. */
    uint64_t *new_cache = (uint64_t *)realloc(prime_cache, cnt * sizeof(uint64_t));
    if (!new_cache) return;   /* In a production build handle OOM. */
    prime_cache = new_cache;
    prime_cnt   = cnt;
    prime_limit = limit;

    /* Fill cache. */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_cache[idx++] = (uint64_t)i;
        }
    }

    free(is_prime);
}



/* ------------------------------------------------------------------ */
/*  Ensure that the cache contains at least 'needed' primes.  The     */
/*  sieve is grown until we have enough primes.  A simple heuristic   */
/*  is used: we grow the sieve limit until it is at least               */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed) return;

    /* Rough upper bound for the n-th prime: n*(log n + log log n).
     * For simplicity we use a generous multiplier (20) that works for
     * the required n ≤ ~10^4. */
    size_t new_limit = needed * 20 + 100;

    /* If the current limit is already enough, keep it. */
    if (prime_limit >= new_limit) new_limit = prime_limit;

    /* Generate primes up to the new limit.  This might be done more than
     * once if the limit proved insufficient. */
    while (1) {
        generate_primes_up_to(new_limit);
        if (prime_cnt >= needed) break;
        /* Not enough primes yet – double the limit. */
        new_limit *= 2;
    }
}



/* ------------------------------------------------------------------ */
/*  Main sequence function.  It is safe to call with arbitrary n ≥ 1   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* Undefined for n = 0 */

    /* We need the n-th, (n+1)-th and (n+2)-th primes. */
    ensure_primes((size_t)n + 2);

    seq_t p1 = (seq_t)prime_cache[(size_t)n   - 1];
    seq_t p2 = (seq_t)prime_cache[(size_t)n   ];
    seq_t p3 = (seq_t)prime_cache[(size_t)n + 1];

    return p1 + p2 + p3;
}
