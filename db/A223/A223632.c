/*
 * A223632  Number of n X 3 0..1 arrays with rows, antidiagonals and columns
 *          unimodal.
 * https://oeis.org/A223632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n > 7 , a(1) = 7, a(2) = 49, a(3) = 229,
 *            a(4) = 801, a(5) = 2297, a(6) = 5699, a(7) = 12657
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 63, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cache[0] holds a(1) */
static size_t  cache_capacity = 0;    /* allocated array size (in seq_t) */
static idx_t   cache_max = 0;         /* number of terms currently computed */

static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;          /* already computed */

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        if (cache == NULL) abort();
        cache_capacity = (size_t)n;
    } else if ((size_t)n > cache_capacity) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Base values for n = 1..7 (indices 0..6) */
    static const seq_t base[7] = {
        (seq_t)7,   (seq_t)49,   (seq_t)229,   (seq_t)801,
        (seq_t)2297, (seq_t)5699, (seq_t)12657
    };

    /* Fill base values if not already present */
    for (idx_t i = cache_max; i < n && i < 7; ++i) {
        cache[i] = base[i];
    }

    /* Compute terms using the recurrence for i >= 7 */
    for (idx_t i = (idx_t)(cache_max > 7 ? cache_max : 7); i < n; ++i) {
        cache[i] =  (seq_t)7 * cache[i-1]
                  - (seq_t)21 * cache[i-2]
                  + (seq_t)35 * cache[i-3]
                  - (seq_t)35 * cache[i-4]
                  + (seq_t)21 * cache[i-5]
                  - (seq_t)7  * cache[i-6]
                  +            cache[i-7];
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; this case is unused */
    ensure_cache(n);
    return cache[n-1];
}
