/*
 * A124138  a(n) = A000265(3*(a(n-1)+a(n-2))/2 +1) starting at a(1)=1, a(2)=3.
 * https://oeis.org/A124138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = odd_part(3*(a(n-1)+a(n-2))/2 + 1), a(1)=1,a(2)=3
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 62, u64 n <= 120,
 *            u128 n <= 288, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/odd_part.h"

/* odd_part: remove all factors of 2 from x */

/* static cache for memoisation */
static seq_t *cache      = NULL; /* array of computed terms */
static size_t cached_len = 0;     /* number of items currently stored */
static size_t cache_cap  = 0;     /* allocated capacity */

/* ensure the cache has capacity for at least 'size' elements */
static inline void ensure_cache_size(size_t size)
{
    if (size <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < size)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        abort(); /* allocation failed */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for indices below 1 */

    size_t target = (size_t)n;
    ensure_cache_size(target);

    /* initialise the first two terms if necessary */
    if (cached_len == 0) {
        cache[0] = 1;
        cached_len = 1;
    }
    if (cached_len < 2) {
        cache[1] = 3;
        cached_len = 2;
    }

    /* compute any terms up to 'target' */
    for (size_t i = cached_len; i < target; ++i) {
        seq_t sum_half = (cache[i - 1] + cache[i - 2]) / 2; /* sum is even */
        seq_t val = sum_half * 3 + 1;
        cache[i] = odd_part(val);
    }
    cached_len = target;
    return cache[n - 1];
}
