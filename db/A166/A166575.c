/*
 * A166575  Primes p>=5 with the property: if Prime(k)<p/2<Prime(k+1), then
 *          p>=Prime(k)+ Prime(k+1).
 * https://oeis.org/A166575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166575: primes p ≥ 5 such that
   if Prime(k) < p/2 < Prime(k+1) then p ≥ Prime(k)+Prime(k+1).
   The routine generates primes incrementally, tests the inequality
   using the primes immediately below and above p/2, and memoises the
   qualifying terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

static uint64_t *prime_list = NULL;          /* primes up to current limit   */
static size_t   prime_cnt   = 0;             /* number of primes in prime_list   */
static size_t   next_prime  = 0;             /* index of next prime to test      */
static uint64_t prime_limit = 0;             /* current upper bound for primes   */

static seq_t *cache       = NULL;            /* cached sequence terms          */
static size_t cache_cnt   = 0;               /* number of terms stored in cache */
static size_t cache_cap   = 0;               /* capacity of cache array        */

static bool initialized = false;             /* one‑time initialization flag   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                                 /* offset 1 */

    if (!initialized) {                                   /* initial setup */
        initialized = true;
        prime_limit = 1000;
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        next_prime  = 0;
        cache_cnt   = 0;
        cache_cap   = 0;
    }

    /* Keep generating primes until the cache has at least n terms. */
    while (cache_cnt < (size_t)n) {
        /* Load more primes if we have exhausted the current list. */
        if (next_prime >= prime_cnt) {
            prime_limit <<= 1;                     /* double the limit */
            uint64_t *tmp; size_t tmp_cnt;
            build_primes_upto(prime_limit, &tmp, &tmp_cnt);
            free(prime_list);                      /* free old list   */
            prime_list = tmp;
            prime_cnt  = tmp_cnt;
            continue;                               /* retry with new list */
        }

        uint64_t p = prime_list[next_prime++];
        if (p < 5) continue;                       /* skip 2 and 3 */

        /* Find the prime immediately below p/2.  Use multiplication
           by 2 instead of division to avoid truncation. */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] * 2ULL < p)
                lo = mid + 1;
            else
                hi = mid;
        }
        size_t lower_idx = lo - 1;                /* prime < p/2 */

        /* Find the prime immediately above p/2. */
        lo = 0; hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_list[mid] * 2ULL <= p)
                lo = mid + 1;
            else
                hi = mid;
        }
        size_t upper_idx = lo;                    /* prime > p/2 */

        if (lower_idx >= prime_cnt || upper_idx >= prime_cnt)
            continue;                               /* safety guard */

        uint64_t sum = prime_list[lower_idx] + prime_list[upper_idx];

        if (p >= sum) {                            /* qualifies */
            ensure_capacity(cache_cnt + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_cnt++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
