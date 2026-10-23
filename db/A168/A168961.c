/*
 * A168961  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168961  
   a(0) = 1  
   For 1 <= n <= 21:   a(n) = 2 + 42 * Σ_{k=0}^{n-1} a(k)  
   For n >= 22:       a(n) = 42 * Σ_{k=1}^{21} a(n-k) - 903 * a(n-22)  */
#include <stdlib.h>
#include <stddef.h>

/* dynamic cache of computed terms */
static seq_t *cache   = NULL;   /* array of a(n) values */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_len = 0;    /* number of computed terms, i.e. one past
                                * the largest index stored in cache   */
static seq_t cumulative_sum = 0;/* Σ_{k=0}^{i-1} a(k) during construction */

/* ensure that the cache can hold values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* fill cache with terms up to index n (inclusive) */
static void fill_cache(idx_t n)
{
    /* initialize first term if cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cumulative_sum = cache[0];
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i <= 21) {                 /* recurrence for the first 22 terms */
            val = (seq_t)2 + ((seq_t)42 * cumulative_sum);
            cumulative_sum += val;
        } else {                       /* recurrence for n >= 22 */
            seq_t sum21 = 0;
            for (idx_t j = 1; j <= 21; ++j)
                sum21 += cache[i - j];
            val = ((seq_t)42 * sum21) - ((seq_t)903 * cache[i - 22]);
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    /* lazy computation: compute up to requested index if necessary */
    if (n >= (idx_t)cache_len) {
        ensure_cache(n);
        fill_cache(n);
    }
    return cache[(size_t)n];
}
