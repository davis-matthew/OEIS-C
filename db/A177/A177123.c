/*
 * A177123  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=7, k=1 and l=1.
 * https://oeis.org/A177123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(1)=7; for n>=2: a(n)=∑_{p=0}^{n-1} a(p)*a(n-1-p)+n+1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed a(n) values */
static idx_t  cache_len = 0;       /* number of valid entries in cache   */
static size_t cache_cap = 0;       /* allocated capacity of cache         */

static void ensure_cache(idx_t n)
{
    /* Ensure cache has entries up to index n (inclusive) */
    if (n < cache_len) return;

    /* Grow capacity if needed */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory */
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute new terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)7;
        } else {
            seq_t sum = 0;
            /* convolution contribution */
            for (idx_t p = 0; p < i; ++p) {
                sum += cache[p] * cache[i - 1 - p];
            }
            /* Add (i + 1) as per the recurrence (k=1, l=1) */
            cache[i] = sum + (seq_t)(i) + 1;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
