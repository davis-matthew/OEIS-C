/*
 * A170003  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of reduced words of length n in the Coxeter group
 *            on 42 generators with relations S_i^2 = (S_i S_j)^35 = I. The
 *            sequence satisfies the linear recurrence derived from its
 *            generating function: a(n) = 40 * (a(n-1)+a(n-2)+...+a(n-34)) -
 *            820 * a(n-35) for n >= 36 with initial values a(0)...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
static seq_t *cache = nullptr;
static idx_t cache_len = 0;     /* number of values currently stored */
static idx_t cache_capacity = 0;
static void ensure_capacity(idx_t needed)
{
    if (cache_capacity >= needed)
        return;
    seq_t *new_cache = new seq_t[needed];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete [] cache;
    cache = new_cache;
    cache_capacity = needed;
}
#else
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_capacity = 0;
static void ensure_capacity(idx_t needed)
{
    if (cache_capacity >= needed)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) /* allocation failed – abort */
        exit(1);
    cache = new_cache;
    cache_capacity = needed;
}
#endif

seq_t a(idx_t n)
{
    if (n + 1 > cache_capacity)
        ensure_capacity(n + 1);

    if (cache_len == 0) {          /* initialize cache[0] = 1 */
        cache[0] = 1;
        cache_len = 1;
    }

    /* compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 34) {
            /* a(i) = 2 + 40 * sum_{k=0}^{i-1} a(k) */
            seq_t sumPrev = 0;
            for (idx_t j = 0; j < i; ++j)
                sumPrev += cache[j];
            cache[i] = 2 + 40 * sumPrev;
        } else if (i == 35) {
            /* a(35) = 1 + 40 * sum_{k=0}^{34} a(k) - 820 * a(0) */
            seq_t sumPrev = 0;
            for (idx_t j = 0; j < 35; ++j)
                sumPrev += cache[j];
            cache[i] = 1 + 40 * sumPrev - 820 * cache[0];
        } else { /* i >= 36 */
            seq_t sumPrev = 0;
            for (idx_t j = i - 34; j < i; ++j)   /* sum of the previous 34 terms */
                sumPrev += cache[j];
            cache[i] = 40 * sumPrev - 820 * cache[i - 35];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
