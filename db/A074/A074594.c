/*
 * A074594  Number of distinct prime factors of numbers that are not prime
 *          powers.
 * https://oeis.org/A074594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(A024619(n)) where A024619(n) is the n-th integer
 *            that is not a prime power
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Memoisation buffer for the computed sequence values. */
static seq_t *cache      = NULL;   /* term cache */
static idx_t cache_len   = 0;      /* number of terms stored */
static idx_t cache_cap   = 0;      /* allocated cache size          */

/* The largest integer examined so far (to continue the search). */
static uint64_t last_checked = 1u;

/* Ensure that cache holds at least `needed` entries. */
static void ensure_cache_capacity(idx_t needed) {
    if (needed <= cache_cap)
        return;
    /* Grow cache exponentially. */
    idx_t new_cap = (cache_cap > 0) ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Count distinct prime factors of n (bigOmega).  Returns 0 for n==1. */
static int count_distinct_primes(uint64_t n) {
    if (n < 2)
        return 0;
    int count = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            ++count;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)        /* remaining prime factor > sqrt(original n) */
        ++count;
    return count;
}

/* Public sequence function.  Indexing starts at 1. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* undefined for 0, but harmless */

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        ++last_checked;                       /* next candidate integer */

        int omega = count_distinct_primes(last_checked);
        if (omega == 1)                        /* prime power, skip */
            continue;

        /* Store result in cache. */
        if (cache_len >= cache_cap)
            ensure_cache_capacity(cache_len + 1);
        cache[cache_len] = (seq_t)omega;
        ++cache_len;
    }
    return cache[n - 1];
}
