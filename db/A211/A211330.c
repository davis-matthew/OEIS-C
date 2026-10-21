/*
 * A211330  Number of (n+1) X (n+1) -5..5 symmetric matrices with every 2 X 2
 *          subblock having sum zero and three distinct values.
 * https://oeis.org/A211330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2) - 31*a(n-3) + 39*a(n-4) + 56*a(n-5) -
 *            116*a(n-6) - 14*a(n-7) + 115*a(n-8) - 34*a(n-9) - 30*a(n-10) +
 *            12*a(n-11) Initial values (offset 1):
 *            a(1)=46,a(2)=90,a(3)=172,a(4)=316,a(5)=588,
 *            a(6)=1070,a(7)=1974,a(8)=3608,a(9)=6670,a(10)=12310,a(11)=22930
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for computed terms.  cache[0] == a(1). */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* size of allocated array */

/* Ensure the cache has room for at least `need` terms. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 11 terms. */
static void init_cache(void)
{
    if (cache_cnt >= 11)
        return;
    ensure_cache(11);
    static const seq_t init[11] = {
        (seq_t)46,  (seq_t)90,  (seq_t)172,  (seq_t)316,  (seq_t)588,
        (seq_t)1070,(seq_t)1974,(seq_t)3608,(seq_t)6670,(seq_t)12310,(seq_t)22930
    };
    for (size_t i = 0; i < 11; ++i)
        cache[i] = init[i];
    cache_cnt = 11;
}

/* Public function to return a(n).  n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n = 0, but safe */

    init_cache();

    /* If the requested index is already cached, just return it. */
    if ((idx_t)n <= (idx_t)cache_cnt)
        return cache[n-1];

    /* Compute missing terms up to n. */
    size_t new_cnt = (size_t)n;
    ensure_cache(new_cnt);

    for (idx_t k = (idx_t)cache_cnt + 1; k <= n; ++k) {
        idx_t i = (idx_t)k;   /* 1‑based index for the term being computed */

        /* Since we need a(i-1) … a(i-11), the cache already contains those. */
        seq_t pos = (seq_t)5  * cache[i-2]   /* a(i-1) */
                 + (seq_t)39 * cache[i-5]   /* a(i-4) */
                 + (seq_t)56 * cache[i-6]   /* a(i-5) */
                 + (seq_t)115 * cache[i-9]  /* a(i-8) */
                 + (seq_t)12 * cache[i-12] ;/* a(i-11) */

        seq_t neg = cache[i-3]               /* a(i-2) */
                 + (seq_t)31 * cache[i-4]   /* a(i-3) */
                 + (seq_t)116 * cache[i-7]  /* a(i-6) */
                 + (seq_t)14 * cache[i-8]   /* a(i-7) */
                 + (seq_t)34 * cache[i-10] /* a(i-9) */
                 + (seq_t)30 * cache[i-11] ;/* a(i-10) */

        /* The recurrence guarantees the result is non‑negative. */
        cache[i-1] = pos - neg;
    }

    cache_cnt = new_cnt;
    return cache[n-1];
}
