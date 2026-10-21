/*
 * A070087  P(n) > P(n+1) where P(n) (A006530) is the largest prime factor of
 *          n.
 * https://oeis.org/A070087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that the largest prime factor of
 *            k is greater than that of k+1. The definition follows OEIS
 *            A070087
 * Range:     u8 n <= 121, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"

static seq_t *cache = NULL;   /* array of found terms          */
static size_t  cache_len  = 0;/* number of terms stored         */
static size_t  cache_cap  = 0;/* allocated capacity              */
static idx_t   next_k     = 2;/* smallest k to test next        */

/* ensure capacity for at least need elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined, but harmless */

    /* grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* test current k for the P(k) > P(k+1) condition   */
        uint64_t pk  = largest_prime_factor((uint64_t)next_k);
        uint64_t pk1 = largest_prime_factor((uint64_t)(next_k + 1));

        if (pk > pk1) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
