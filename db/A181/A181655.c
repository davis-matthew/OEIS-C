/*
 * A181655  Expansion of (1+2x-x^3+x^4)/(1-4x^2+3x^4).
 * https://oeis.org/A181655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation of OEIS A181655:
   a(0)=1, a(1)=2, a(2)=4, a(3)=7, a(4)=14,
   a(n)=4*a(n-2)-3*a(n-4)   for n>=5   (derived from (1+2x-x^3+x^4)/((1-x^2)(1-3x^2)) */
#include <stdlib.h>

static seq_t *cache = NULL;         /* cache of computed values       */
static idx_t  cache_len = 0;        /* number of entries already stored */
static idx_t  cache_cap = 0;        /* allocated capacity             */

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* enlarge capacity if needed */
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= n) new_cap <<= 1;

    if (new_cap != cache_cap) {
        if (cache) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        } else {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) abort();
        }
        cache_cap = new_cap;
    }

    /* fill cache from the first missing term up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[i] = 1;
        else if (i == 1)     cache[i] = 2;
        else if (i == 2)     cache[i] = 4;
        else if (i == 3)     cache[i] = 7;
        else if (i == 4)     cache[i] = 14;
        else {
            seq_t v1 = cache[i-2];
            seq_t v2 = cache[i-4];
            cache[i] = (seq_t)4 * v1 - (seq_t)3 * v2;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
