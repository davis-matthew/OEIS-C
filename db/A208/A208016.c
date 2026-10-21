/*
 * A208016  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 21*a(n-3) + 9*a(n-4) - 27*a(n-5) -
 *            81*a(n-6) for n > 8 Initial values: a(1)=16, a(2)=256, a(3)=784,
 *            a(4)=4489, a(5)=15376, a(6)=43264, a(7)=167281, a(8)=609961
 *            Implements memoised calculation with a static cache. The cache
 *            is grown as needed and always contains all valu...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] unused, cache[1] = a(1) */
static idx_t  cache_capacity = 0;   /* allocated array size (>=1) */
static idx_t  cache_computed  = 0;   /* highest n whose a(n) is in cache */

/* ensure that cache is allocated for indices up to at least n (inclusive)
   and that a(1)...a(n) are computed. */
static void ensure_cache(idx_t n)
{
    /* allocate or grow the buffer to accommodate indices 1..n */
    if (cache == NULL) {
        size_t cap = (size_t)(n + 1 > (idx_t)9 ? (idx_t)n + 1 : (idx_t)9);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache_capacity = cap;
    } else if ((idx_t)(cache_capacity) <= n) {
        size_t new_cap = (size_t)(n + 1 > (idx_t) (cache_capacity*2)
                                 ? (idx_t)n + 1
                                 : cache_capacity * 2);
        if (new_cap < (size_t)9) new_cap = 9;
        cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_capacity = new_cap;
    }

    /* initialise base values if not already done */
    if (cache_computed < (idx_t)8) {
        idx_t start = cache_computed + 1;
        if (start < 1) start = 1;
        if (start <= 8) {
            /* Constants are small; cast to seq_t is safe. */
            if (((size_t)start) <= (size_t)1) cache[1] = (seq_t)16;
            if (((size_t)start) <= (size_t)2) cache[2] = (seq_t)256;
            if (((size_t)start) <= (size_t)3) cache[3] = (seq_t)784;
            if (((size_t)start) <= (size_t)4) cache[4] = (seq_t)4489;
            if (((size_t)start) <= (size_t)5) cache[5] = (seq_t)15376;
            if (((size_t)start) <= (size_t)6) cache[6] = (seq_t)43264;
            if (((size_t)start) <= (size_t)7) cache[7] = (seq_t)167281;
            if (((size_t)start) <= (size_t)8) cache[8] = (seq_t)609961;
            if (cache_computed < (idx_t)8) {
                idx_t new_computed = (n < (idx_t)8 ? n : (idx_t)8);
                if (new_computed > cache_computed)
                    cache_computed = new_computed;
            }
        }
        /* If n < 8 we return early; further steps are unnecessary. */
        if (n <= (idx_t)8) return;
    }

    /* compute remaining values using the recurrence, starting at
       max(cache_computed+1, 9) */
    idx_t i = cache_computed + 1;
    if (i < (idx_t)9) i = (idx_t)9;
    for (; i <= n; ++i) {
        /* all indices i-1 .. i-6 are guaranteed to be already computed */
        seq_t a1 = cache[(size_t)(i - 1)];
        seq_t a2 = cache[(size_t)(i - 2)];
        seq_t a3 = cache[(size_t)(i - 3)];
        seq_t a4 = cache[(size_t)(i - 4)];
        seq_t a5 = cache[(size_t)(i - 5)];
        seq_t a6 = cache[(size_t)(i - 6)];
        cache[(size_t)i] = a1 + 3 * a2 + 21 * a3 + 9 * a4
                           - 27 * a5 - 81 * a6;
    }
    if (n > cache_computed) cache_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset 1 */
    ensure_cache(n);
    return cache[(size_t)n];
}
