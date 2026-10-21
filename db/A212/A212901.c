/*
 * A212901  Number of (w,x,y,z) with all terms in {0,...,n} and equal
 *          consecutive gap sizes.
 * https://oeis.org/A212901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) Initial values:
 *            a(0)=1, a(1)=4, a(2)=13, a(3)=26, a(4)=45, a(5)=66
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_used = 0;

static const seq_t init_vals[6] = { (seq_t)1, (seq_t)4, (seq_t)13,
                                   (seq_t)26, (seq_t)45, (seq_t)66 };

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n >= cache_used) {
        for (idx_t i = cache_used; i <= n; ++i) {
            if (i <= 5) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = cache[i-1] + cache[i-2]
                           - cache[i-4] - cache[i-5]
                           + cache[i-6];
            }
        }
        cache_used = n + 1;
    }

    return cache[n];
}
