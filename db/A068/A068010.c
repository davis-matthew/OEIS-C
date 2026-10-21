/*
 * A068010  Number of subsets of {1,2,3,...,n} that sum to 0 mod 3.
 * https://oeis.org/A068010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1
 * a(n) = 2*a(n-1) - 2^((n-1)/3)   if (n-1) is divisible by 3
 *        2*a(n-1)                  otherwise
 *   (valid for n ≥ 1)
 *
 * The function memoises computed terms so that calls in arbitrary
 * order are O(max(n)) overall.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;  /* array of a(i) for i < cache_len   */
    static size_t cache_len = 0;   /* number of cached terms already      */
    static size_t cache_cap = 0;   /* capacity of malloc'ed array         */

    /* initialise cache if first time */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory sentinel */
    }

    /* grow cache to accommodate n, if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* cache the first value a(0) if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* if n is already cached, return it */
    if (n < (idx_t)cache_len) return cache[(size_t)n];

    /* compute terms up to n, filling the cache */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] << 1;         /* 2*a(i-1)                */
        if ( ((i-1) % 3) == 0 ) {            /* subtract 2^((i-1)/3)   */
            size_t k = (i-1) / 3;
            val -= ((seq_t)1 << k);          /* safe for builtin or big */
        }
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
