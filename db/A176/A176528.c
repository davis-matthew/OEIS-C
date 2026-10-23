/*
 * A176528  a(2*n) = n*a(n); a(2*n+1) = n*a(n) + a(n+1), with a(1) = 1.
 * https://oeis.org/A176528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2n) = n * a(n); a(2n+1) = n * a(n) + a(n+1)
 * Range:     u8 n <= 21, u16 n <= 71, u32 n <= 352, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache holding already computed terms */
static seq_t *cache = NULL;
static idx_t cache_max = 0;

/* ensure we have entries up to n in the cache */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise cache with room for up to n */
        size_t cap = (size_t)n + 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) abort();
        cache[0] = 0;           /* unused */
        cache[1] = (seq_t)1;    /* base value */
        cache_max = 1;
    }
    if (n <= cache_max) return;          /* already ready */

    /* expand the allocated array if necessary */
    size_t new_cap = (size_t)n + 2;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();
    cache = tmp;

    /* compute missing entries */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if ((i & 1) == 0) {                 /* i = 2*m */
            idx_t m = i >> 1;
            cache[i] = ((seq_t)m) * cache[m];
        } else {                             /* i = 2*m + 1 */
            idx_t m = i >> 1;
            cache[i] = ((seq_t)m) * cache[m] + cache[m + 1];
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined; return 0 for safety */
    ensure_cache(n);
    return cache[n];
}
