/*
 * A129770  a(0) = 0, a(1) = 1; for n>0, a(2*n) = 3*a(2*n-1), a(2*n+1) =
 *          3*a(2*n) - 2*a(n-1).
 * https://oeis.org/A129770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; for n > 0: a(2*n) = 3*a(2*n - 1) a(2*n + 1)
 *            = 3*a(2*n) - 2*a(n - 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of values currently cached */
static size_t cache_cap = 0;   /* capacity of the allocated array */

/* resize the cache so it can hold at least `need` entries */
static void grow_cache(idx_t need)
{
    if ((idx_t)cache_cap >= need) return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 2;
    while ((idx_t)new_cap < need) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort(); /* allocation failure */
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        grow_cache(2);          /* need indices 0 and 1 */
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    if (n < cache_len) return cache[n];

    /* grow cache to at least n+1 entries */
    grow_cache(n + 1);

    for (idx_t k = cache_len; k <= n; ++k) {
        if ((k & 1) == 0) {          /* k is even: k = 2 * (k/2) */
            cache[k] = (seq_t)3 * cache[k - 1];
        } else {                     /* k is odd: k = 2 * ((k-1)/2) + 1 */
            idx_t t = (k - 1) / 2;   /* corresponds to n in the formula */
            /* guard against underflow when t == 0, but k≥3 here so t≥1 */
            cache[k] = (seq_t)3 * cache[k - 1] - (seq_t)2 * cache[t - 1];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
