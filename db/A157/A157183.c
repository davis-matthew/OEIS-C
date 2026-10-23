/*
 * A157183  Primes in A028883, p=m^2-7, such that following prime is m^2+1.
 * https://oeis.org/A157183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime p such that p = m² – 7 is prime and the next prime after p is m² + 1. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* static cache for already found terms */
static seq_t *cache  = NULL;
static size_t cache_sz  = 0;
static size_t cache_cap = 0;
static idx_t   cur_m    = 3;          /* we start checking from m = 4 (m=3 gives a negative p) */

/* make sure the cache can hold at least 'need' terms */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* the OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; n=0 is undefined, return 0 as placeholder */

    /* generate terms until we have at least n of them */
    while (cache_sz < n) {
        ++cur_m;
        seq_t m_sq = (seq_t)cur_m * (seq_t)cur_m;
        seq_t p1   = m_sq - 7;   /* candidate prime */
        seq_t p2   = m_sq + 1;   /* following candidate */

        if (p1 < 2)
            continue;
        if (!is_prime((idx_t)p1))
            continue;

        uint64_t next = next_prime((uint64_t)p1);
        if (next == (uint64_t)p2) {
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = p1;
        }
    }
    return cache[n - 1];
}
