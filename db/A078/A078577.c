/*
 * A078577  Sum of all prime factors of the average of n-th twin prime pair.
 * https://oeis.org/A078577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of prime factors (with multiplicity) of the average
 *            of the n‑th twin prime pair. That is, a(n) = A001414( (p_n +
 *            (p_n + 2)) / 2 ) where p_n and p_n+2 are the primes of the n‑th
 *            twin prime pair
 * Range:     u8 n <= 53, u16 n <= 3787, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"    /* build_primes_upto */
#include "../utils/_smallest_prime_factor.h"   /* _smallest_prime_factor */
#include "../utils/seq_alloc.h"           /* seq_alloc */
#include "../utils/seq_realloc.h"         /* seq_realloc */

/* State for memoisation */
static idx_t max_processed = 0;       /* highest n computed so far          */
static seq_t *cache       = NULL;     /* a(i) values, 0‑based                */
static size_t cache_cap   = 0;        /* capacity of cache array             */
static uint64_t *primes   = NULL;     /* array of primes up to current limit */
static size_t primes_cnt   = 0;        /* number of primes in the array       */
static idx_t sieve_limit  = 0;        /* limit up to which primes are built  */

/* Sum of prime factors (multiplicity) of an uint64_t number */
static seq_t sum_prime_factors_uint64(uint64_t n) {
    seq_t sum = 0;
    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        sum += p;
        n /= p;
    }
    return sum;
}

/* Ensure that values up to a(n) are available in the cache
   and that the prime sieve contains at least n twin primes. */
static void ensure_upto(idx_t n) {
    if (n <= max_processed) {    /* nothing new to compute */
        return;
    }

    /* Grow the prime sieve until we have at least n twin primes. */
    if (sieve_limit == 0) {
        sieve_limit = 3;          /* start with a small limit */
    }
    while (1) {
        /* Rebuild primes up to the current limit. */
        if (primes) free(primes);
        build_primes_upto(sieve_limit, &primes, &primes_cnt);

        /* Count twin primes in this list. */
        idx_t twin_cnt = 0;
        for (size_t i = 0; i + 1 < primes_cnt; ++i) {
            if (primes[i + 1] == primes[i] + 2) {
                ++twin_cnt;
            }
        }

        if (twin_cnt >= n) {
            break;                 /* enough twin primes found */
        }
        sieve_limit *= 2;           /* enlarge limit and retry */
    }

    /* Allocate or grow the cache array if needed. */
    if (cache_cap < (size_t)n) {
        seq_t *new_cache = seq_realloc(cache, cache_cap, (size_t)n);
        if (!new_cache) abort();   /* out of memory */
        cache = new_cache;
        cache_cap = (size_t)n;
    }

    /* Compute a(i) values for new twin primes beyond max_processed. */
    idx_t count = 0;                  /* current twin pair index (1‑based) */
    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        if (primes[i + 1] == primes[i] + 2) {
            ++count;
            if (count > max_processed) {
                /* The average of the twin primes is simply primes[i] + 1. */
                uint64_t avg = primes[i] + 1;
                cache[count - 1] = sum_prime_factors_uint64(avg);
            }
            if (count >= n) {
                break; /* computed all needed values */
            }
        }
    }
    max_processed = n;
}

seq_t a(idx_t n) {
    if (n < 1) {
        return 0;                 /* undefined for n<1, return 0 */
    }
    ensure_upto(n);
    return cache[n - 1];
}
