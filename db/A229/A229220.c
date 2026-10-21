/*
 * A229220  a(n) = a(n-1)^2 + (-1)^n with a(1)=1.
 * https://oeis.org/A229220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + (−1)^n , a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;     /* array of computed terms */
static idx_t  cached_len = 0;   /* number of allocated slots */
static idx_t  last_computed = 0;/* largest n for which a(n) is known */

seq_t a(idx_t n)
{
    /* Offset is 1. For n < 1 the definition does not exist. */
    if (n < 1u) return (seq_t)0;

    /* Base case */
    if (n == 1u) return (seq_t)1;

    /* If the value has already been computed, return it immediately. */
    if (n <= last_computed) {
        return cache[n - 1];
    }

    /* Allocate or grow the cache so that it can hold a(n). */
    if (cache == NULL) {
        cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (!cache) abort();
        cached_len = n;
    } else if (n > cached_len) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cached_len = n;
    }

    /* Initialize the cache for the first term if it hasn't been done yet. */
    if (last_computed == 0u) {
        cache[0] = (seq_t)1;
        last_computed = 1u;
    }

    /* Compute successive terms up to n. */
    seq_t cur = cache[last_computed - 1u];
    idx_t start = (last_computed + 1u > 2u) ? last_computed + 1u : 2u;
    for (idx_t i = start; i <= n; ++i) {
        cur = cur * cur;          /* a(i-1)^2 */
        if (i & 1u) {             /* i is odd -> add (-1) */
            cur = cur - (seq_t)1;
        } else {                  /* i is even -> add (+1) */
            cur = cur + (seq_t)1;
        }
        cache[i - 1u] = cur;
    }
    last_computed = n;
    return cache[n - 1u];
}
