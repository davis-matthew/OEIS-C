/*
 * A078514  Numbers k such that the smallest prime dividing k is the largest
 *          exponent in the factorization of k.
 * https://oeis.org/A078514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A078514.
   The sequence consists of all integers k ≥ 2 such that
   the smallest prime divisor of k equals the largest exponent
   occurring in the prime factorisation of k.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor.h"   /* provides factor(uint64_t,n,pr,exp,cnt) */

static seq_t *cache = NULL;          /* cache of already found terms          */
static size_t cache_len = 0;         /* number of cached terms                */
static size_t cache_cap = 0;         /* capacity of cache array               */
static uint64_t next_k = 2;          /* next integer to test                   */

/* Ensure that the cache can hold at least n elements. */
static void ensure_cache_cap(size_t n)
{
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return 1 if integer k satisfies the condition, 0 otherwise. */
static int good_k(uint64_t k)
{
    if (k < 2) return 0;      /* k=1 is not part of the sequence */

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor(k, primes, exps, &cnt);    /* factorisation provided by utils */

    if (cnt == 0) return 0;  /* should not occur for k ≥ 2 */

    uint64_t sp = primes[0];          /* smallest prime divisor      */
    uint64_t emax = 0;                /* largest exponent in factorisation */
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > emax) emax = exps[i];

    return sp == emax;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard against n==0 */

    /* Return cached result if available. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Extend cache until we have n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next integer satisfying the property. */
        do {
            if (good_k(next_k))
                break;
            ++next_k;
        } while (1);

        /* Store the found term in the cache. */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = (seq_t)next_k;
        ++cache_len;
        ++next_k;   /* next candidate for subsequent calls */
    }

    return cache[n - 1];
}
