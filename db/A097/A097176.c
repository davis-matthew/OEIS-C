/*
 * A097176  a(n) = Sum_{k=0..n} binomial(floor((n+1)/2), floor((k+1)/2)) *
 *          5^k.
 * https://oeis.org/A097176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(floor((n+1)/2), floor((k+1)/2)) *
 *            5^k Equivalent recurrence (for n ≥ 5): a(0)=1, a(1)=6, a(2)=31,
 *            a(3)=186, a(4)=811
 *            a(n)=a(n‑1)+51·a(n‑2)-51·a(n‑3)-650·a(n‑4)+650·a(n‑5) The
 *            recurrence holds for all n≥5 and yields the sequence exactly
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of initialized values */
    static size_t cache_cap = 0;   /* capacity of the cache */

    /* base values (hard‑coded but trivial to compute from the definition) */
    const seq_t init_vals[5] = { (seq_t)1, (seq_t)6, (seq_t)31,
                                 (seq_t)186, (seq_t)811 };

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();       /* allocation failure */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
        cache_len = 5;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 5;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 5) {
            val = cache[i];           /* already initialised */
        } else {
            val = cache[i-1];
            val += (seq_t)51 * cache[i-2];
            val -= (seq_t)51 * cache[i-3];
            val -= (seq_t)650 * cache[i-4];
            val += (seq_t)650 * cache[i-5];
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
