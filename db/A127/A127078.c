/*
 * A127078  a(0)=1. a(n) = a(n-1) + (product of the earlier terms {among terms
 *          a(0) through a(n-1)} which are coprime to n).
 * https://oeis.org/A127078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A127078
 *  a(0) = 1
 *  a(n) = a(n-1) + product{ a(k) : 0 ≤ k < n and gcd(a(k), n) = 1 }
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"     /* seq_t gcd(seq_t a, seq_t b) */

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed a(n) values            */
static size_t cache_cap = 0;     /* size allocated for cache                */
static idx_t  cache_len = 0;     /* number of terms stored (cache_len = n+1) */

/* ensure the cache can hold at least n+1 terms */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n) return;              /* already big enough */

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                       /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* required function */
seq_t a(idx_t n)
{
    /* base case */
    if (n == (idx_t)0) return (seq_t)1;

    /* initialise cache if necessary */
    if (cache_len == 0) {
        ensure_capacity(0);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    if (cache_len <= n) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prod = (seq_t)1;
            for (idx_t j = (idx_t)0; j < i; ++j) {
                /* coprime test: gcd(a(j), i) == 1 */
                if (gcd(cache[j], (seq_t)i) == (seq_t)1) {
                    prod *= cache[j];
                }
            }
            cache[i] = cache[i-1] + prod;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
