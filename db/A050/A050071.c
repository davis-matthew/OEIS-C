/*
 * A050071  a(n) = a(n-1)+a(m), where m=2n-2-2^(p+1) and 2^p<n-1<=2^(p+1), for
 *          n >= 4.
 * https://oeis.org/A050071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 125, u64 n <= 503,
 *            u128 n <= 3078, big any n
 * Helpers:   next_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=1, a(2)=3, a(3)=4; for n>=4
 *   a(n)=a(n-1)+a(m) where   
 *            m = 2n-2-2^(p+1)   and  2^p < n-1 <= 2^(p+1).
 *   (The 2^(p+1) term is the smallest power of two >= n-1.)
 * This implements yet‑to‑see A050071 exactly.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_power_of_two.h"

/* static cache: 1‑based indices, index 0 unused */
static seq_t *cache      = NULL;   /* array of computed values   */
static idx_t  cache_len  = 0;      /* highest index currently stored */
static idx_t  cache_cap  = 0;      /* allocated capacity */

/* ensure that the cache can address indices up to at least 'need' */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    /* grow to at least need, doubling each time */
    idx_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache   = tmp;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------
 *    seq_t a(idx_t n)
 * -------------------------------------------------------------
 * Computes the nth term of A050071.  The function is memoised
 * across calls; calls may come in any order.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined, but safe return */

    /* initialise cache on first use */
    if (cache_len == 0) {
        ensure_cache_cap(3);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache_len = 3;
    }

    /* if already stored, return immediately */
    if (n <= cache_len)
        return cache[n];

    /* extend cache to hold up to n */
    ensure_cache_cap(n);

    /* compute new terms sequentially */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence holds only for n >= 4 */
        if (i < 4) {
            /* base values already in cache */
            continue;
        }

        idx_t x   = i - 1;                       /* n-1  */
        idx_t pw2 = next_power_of_two(x);        /* 2^(p+1) */
        idx_t m   = 2 * i - 2 - pw2;              /* m = 2n-2-2^(p+1) */

        cache[i] = cache[i - 1] + cache[m];
    }

    cache_len = n;
    return cache[n];
}
