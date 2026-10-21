/*
 * A086246  Expansion of (1 + x - sqrt(1 - 2*x - 3*x^2)) / 2 in powers of x.
 * https://oeis.org/A086246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 1 For n > 2: a(n) + a(n‑1) =
 *            Σ_{k=0}^n a(k)a(n‑k) (recurrence from OEIS)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 47, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* dynamic cache of previously computed terms */
static seq_t *cache = NULL;           /* array holding the terms             */
static idx_t cache_size = 0;          /* number of terms currently stored    */
static idx_t cache_cap  = 0;          /* capacity of the allocated array     */

/* ensure that the cache has room for indices up to 'need' (inclusive) */
static void ensure_cache(idx_t need)
{
    if (need < cache_cap) return;                /* already enough */
    idx_t new_cap = cache_cap ? cache_cap : 1;    /* start at 1 if empty */
    while (new_cap <= need) new_cap <<= 1;        /* grow to power of two */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                     /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;

    /* initialise any newly allocated slots to 0 */
    for (idx_t i = cache_size; i < new_cap; ++i) cache[i] = 0;
}

/* compute the nth term of A086246 */
seq_t a(idx_t n)
{
    /* make sure that terms 0..n are known */
    if (n >= cache_size) {
        ensure_cache(n);
        /* initialise missing terms sequentially */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)            cache[i] = 0;
            else if (i == 1)       cache[i] = 1;
            else if (i == 2)       cache[i] = 1;
            else {
                seq_t sum = 0;
                for (idx_t k = 1; k < i; ++k) {   /* a(0) and a(i) are 0, skip */
                    sum += cache[k] * cache[i - k];
                }
                cache[i] = sum - cache[i - 1];
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
