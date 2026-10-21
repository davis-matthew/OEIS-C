/*
 * A214039  a(n) = a(n-1) - floor((a(n-2) + a(n-3))/2), with a(n)=n for n < 3.
 * https://oeis.org/A214039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - floor((a(n-2)+a(n-3))/2), a(0)=0, a(1)=1, a(2)=2
 * Range:     i8 n <= 61, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   floor_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_half.h"

static seq_t *cache = NULL;          /* dynamic array holding computed terms */
static size_t cache_cap = 0;         /* capacity of the array */
static idx_t cache_len = 0;          /* number of terms stored */

/* Ensure the cache can hold at least (n+1) elements */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;                      /* already computed */

    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a production environment you might handle this differently
               (e.g., abort), but for the purposes of the exercise we simply
               let the program terminate on OOM. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 3) {
            cache[(size_t)i] = (seq_t)i;
        } else {
            seq_t sum = cache[(size_t)(i - 2)] + cache[(size_t)(i - 3)];
            cache[(size_t)i] = cache[(size_t)(i - 1)] - floor_half(sum);
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 0)
        return 0;   /* undefined, but avoid negatives */
    ensure_cache(n);
    return cache[(size_t)n];
}
