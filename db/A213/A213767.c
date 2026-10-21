/*
 * A213767  Antidiagonal sums of the convolution array A213765.
 * https://oeis.org/A213767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 35, u64 n <= 79, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213767
   Formula: a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6)
   Offset: 1   (a(1)=1, a(2)=5, a(3)=17, a(4)=47, a(5)=114, a(6)=254) */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* array of computed terms (index 0 unused) */
static size_t cache_cap = 0;  /* capacity (maximum index that can be stored) */
static size_t cache_len = 0;  /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* Under the offset – undefined, return 0 */
    if (n == 0)
        return 0;

    /* Ensure the cache can hold index n */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;
        cache_len = 0;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n + 1;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if ((size_t)n > cache_len) {
        static const seq_t init_vals[7] = {0, 1, 5, 17, 47, 114, 254};
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = init_vals[i];
            } else {
                /* a(i) = 4*a(i-1) - 4*a(i-2) - 2*a(i-3) + 4*a(i-4) - a(i-6) */
                seq_t pos = 4 * cache[i - 1] + 4 * cache[i - 4];
                seq_t neg = 4 * cache[i - 2] + 2 * cache[i - 3] + cache[i - 6];
                cache[i] = pos - neg;
            }
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
