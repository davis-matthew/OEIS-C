/*
 * A063713  Numbers n such that there exist primes p, q, r with n*2 = p - r =
 *          r + q (values of r are given in A063714).
 * https://oeis.org/A063713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063713
 * a(n) = the n-th integer n such that there exist primes p,q,r with
 * 2*n = p - r = r + q.
 * The algorithm precomputes a sieve of Eratosthenes up to a fixed
 * limit (1 000 000), stores all primes up to that limit, and
 * tests each candidate n by iterating over the prime list.
 * The sequence is generated lazily and cached so that calls in any
 * order are fast and no recomputation is required. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Sieve limits: 1 000 000 is enough for all n below about 10 000
 * (since 2*n <= 20 000 and we need primes up to 4*n).               */
#define SIEVE_LIMIT 1000000UL
static unsigned char is_prime[SIEVE_LIMIT + 1];

/* Prime list; number of primes below 1 000 000 is 78 498.             */
#define PRIME_MAX 80000UL
static idx_t primes[PRIME_MAX];
static idx_t prime_count = 0;

/* ------------------------------------------------------------------ */
/* Initialise the sieve and prime list.  The routine is executed
 * only once.                                                        */
static void init_sieve(void)
{
    static bool ready = false;
    if (ready) return;
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            primes[prime_count++] = i;

    ready = true;
}

/* ------------------------------------------------------------------ */
/* Test whether the candidate n satisfies the defining property.    */
static bool good_n(idx_t n)
{
    idx_t S = n * 2ULL;               /* 2*n */
    for (idx_t i = 0; i < prime_count && primes[i] < S; ++i) {
        idx_t r = primes[i];
        idx_t a = S - r;              /* candidate q */
        if (a < 2 || !is_prime[a]) continue;
        idx_t b = S + r;              /* candidate p */
        if (b > SIEVE_LIMIT) return false; /* outside sieve */
        if (!is_prime[b]) continue;
        return true;                  /* found suitable triple */
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Cache of generated sequence values.                                  */
static seq_t *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Ensure the cache can hold at least 'needed' elements.              */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function required by the IOI system.                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0 but harmless */
    init_sieve();

    /* Generate terms until we have n of them.  Cache is reused between
       calls, so later calls are O(1).                           */
    if (cache_len < (size_t)n) {
        ensure_cache((size_t)n);
        idx_t next_candidate = (cache_len == 0) ? 1
                                               : (idx_t)cache[cache_len-1] + 1;
        while (cache_len < (size_t)n) {
            if (good_n(next_candidate))
                cache[cache_len++] = (seq_t)next_candidate;
            ++next_candidate;
        }
    }
    return cache[n-1];
}
