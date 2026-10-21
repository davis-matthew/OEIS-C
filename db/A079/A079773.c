/*
 * A079773  a(n) = 2*a(n-1)+15*a(n-2) with n>0, a(0)=0, a(1)=1.
 * https://oeis.org/A079773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+15*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements currently in cache */
static idx_t cache_capacity = 0;

seq_t a(idx_t n)
{
    if (cache == NULL) {
        /* initialize cache with space for at least n+1 elements */
        cache_capacity = (n < 1) ? 2 : ((idx_t)1 << (sizeof(idx_t)*8)); /* will enlarge */
        if (cache_capacity < (n + 1))
            cache_capacity = n + 1;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory – caller must handle */
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    } else if (n + 1 > cache_capacity) {
        /* grow cache to at least 2 * current size until enough */
        idx_t new_cap = cache_capacity;
        while (n + 1 > new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory – caller must handle */
        cache_capacity = new_cap;
    }

    /* compute missing values up to n using recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] * 2 + cache[i-2] * 15;
    }
    if (n + 1 > cache_size) cache_size = n + 1;

    return cache[n];
}
