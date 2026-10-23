/*
 * A181748  Twin primes not of the form p+2^k where p is prime and k>0.
 * https://oeis.org/A181748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smaller prime of a twin prime pair (p, p+2) such
 *            that there is no prime q with p = q + 2^k for any k > 0
 * Range:     u8 n <= 2, u16 n <= 162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the called function */
seq_t a(idx_t n);

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* current capacity of cache */
static idx_t sieve_limit = 0;     /* prime sieve limit for the last rebuild */

/* Helper to rebuild the cache up to a new sieve limit */
static void rebuild_cache(idx_t new_limit, idx_t need)
{
    /* Build prime sieve */
    bool *is_prime = (bool *)calloc((size_t)new_limit + 1, sizeof(bool));
    if (!is_prime) return;                                       /* safety */
    for (idx_t i = 2; i <= new_limit; ++i) is_prime[i] = true;
    for (idx_t i = 2; (idx_t)(i * i) <= new_limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = false;

    /* Temporary dynamic array for results */
    size_t cap = need > 100 ? need : 100;
    seq_t *tmp = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!tmp) { free(is_prime); return; }   /* safety */

    size_t cnt = 0;
    for (idx_t p = 2; p + 2 <= new_limit; ++p) {
        if (!is_prime[p] || !is_prime[p + 2]) continue;          /* not twin */
        bool ok = true;
        /* Check p = q + 2^k for any prime q */
        for (idx_t k = 1; ; ++k) {
            if (k >= 64) break;                                 /* avoid overflow */
            idx_t shift = 1ULL << k;
            if (shift > p) break;
            idx_t q = p - shift;
            if (q >= 2 && is_prime[q]) { ok = false; break; }
        }
        if (ok) {
            if (cnt == cap) {            /* grow temporary array */
                cap <<= 1;
                tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
            }
            tmp[cnt++] = (seq_t)p;
        }
    }

    /* Replace old cache */
    free(cache);
    cache    = tmp;
    cache_cnt = cnt;
    cache_cap = cap;
    sieve_limit = new_limit;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n==0, return 0 */

    /* Ensure we have at least n terms in the cache */
    if (cache_cnt < n) {
        if (sieve_limit == 0) sieve_limit = 1024;   /* start value */
        while (cache_cnt < n) {
            sieve_limit <<= 1;                 /* double limit */
            rebuild_cache(sieve_limit, n);
        }
    }
    return cache[n - 1];
}
