/*
 * A170280  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies the linear recurrence * a(n) =
 *            29*∑_{k=1}^{40} a(n-k) - 435*a(n-41) + p(n) * where p(n) = 1 if
 *            n = 0 or 41, 2 if 1 ≤ n ≤ 40, and 0 otherwise. * This comes from
 *            the rational generating function in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Global cache for memoisation */
static seq_t *cache = NULL;          /* array of computed terms      */
static idx_t cache_size = 0;         /* number of terms cached       */
static size_t cache_capacity = 0;    /* current allocated size       */
static seq_t sum_last40 = 0;         /* sum of last (up to) 40 terms */

/* Ensure the cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n >= cache_size) {
        /* Grow the array if necessary */
        if ((idx_t)(cache_capacity) <= n) {
            size_t new_cap = (cache_capacity == 0) ? 64 : cache_capacity;
            while ((idx_t)(new_cap) <= n) new_cap <<= 1;

#ifdef __cplusplus
            seq_t *new_cache = new seq_t[new_cap];
            /* copy old elements */
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            /* delete old array */
            delete[] cache;
            cache = new_cache;
#else
            /* realloc for C implementation */
            cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
            cache_capacity = new_cap;
        }

        /* Compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t p;
            if (i == 0)          p = 1;
            else if (i <= 40)    p = 2;
            else if (i == 41)    p = 1;
            else                 p = 0;

            seq_t old = (i >= 41) ? cache[i-41] : 0;

            /* a(i) = 29*sum_last40 - 435*old + p */
            seq_t term = 29 * sum_last40 - 435 * old + p;
            cache[i] = term;

            /* Update sliding window sum for next iteration */
            sum_last40 += term;
            if (i >= 40)
                sum_last40 -= cache[i-40];
        }

        cache_size = n + 1;
    }
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
