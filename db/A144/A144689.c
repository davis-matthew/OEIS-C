/*
 * A144689  A098777 mod 7.
 * https://oeis.org/A144689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=5, a(3)=2, a(4)=2, a(5)=2, a(6)=2, a(7)=4,
 *            a(8)=1, a(9)=6, a(10)=6, a(11)=6, a(12)=6, a(13)=5, a(14)=3,
 *            a(15)=4, a(16)=4, a(17)=4, a(18)=4, a(19)=1; for n>19: a(n)=
 *            (a(n-1) - a(n-18) + a(n-19)) mod 7
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Grow the array to accommodate at least 'needed' elements. */
static void ensure_capacity(seq_t **arr, size_t *capacity, size_t needed)
{
    if (needed <= *capacity) return;
    size_t newcap = (*capacity) ? *capacity : 1;
    while (newcap < needed) newcap <<= 1;
    *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
    *capacity = newcap;
}

seq_t a(idx_t n)
{
    /* Static cache that persists between calls. */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* Highest index that has been filled. */
    static size_t cache_cap   = 0;   /* Allocated capacity. */

    /* Initialise the cache with the first 20 terms. */
    if (cache == NULL) {
        ensure_capacity(&cache, &cache_cap, 20);
        const seq_t init[20] = {
            1, 6, 5, 2, 2, 2, 2, 4,
            1, 6, 6, 6, 6, 5, 3, 4,
            4, 4, 4, 1
        };
        for (idx_t i = 0; i < 20; ++i) cache[i] = init[i];
        max_computed = 19;
    }

    /* Return a value that has already been computed. */
    if (n <= max_computed) return cache[n];

    /* Ensure enough capacity. */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity(&cache, &cache_cap, (size_t)n + 1);

    /* Compute values from max_computed+1 up to n using the recurrence. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* All stored values are 0..6, so int arithmetic is safe. */
        int val = (int)cache[k-1] - (int)cache[k-18] + (int)cache[k-19];
        val %= 7;
        if (val < 0) val += 7;
        cache[k] = (seq_t)val;
    }
    max_computed = n;
    return cache[n];
}
