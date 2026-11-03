/*
 * A158608  a(n) = a(n-1) + 16*a(n-2), starting a(0)=1, a(1)=4.
 * https://oeis.org/A158608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 16*a(n-2), a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;
    static idx_t  last_idx  = 0;   /* last index that has been computed */

    /* initialise cache with the first two terms on first call */
    if (cache == NULL) {
        cache_len = 2;                     /* space for indices 0 and 1 */
        cache     = alloc_seq_array(cache_len);
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)4;
        last_idx  = 1;                      /* highest index currently known */
    }

    /* enlarge the cache if necessary */
    if (n + 1 > cache_len) {
        /* grow_seq_array will allocate a new array of the requested
           length and copy the old contents, leaving the old buffer
           freed if it was allocated by alloc_seq_array. */
        grow_seq_array(&cache, &cache_len, n + 1);
    }

    /* compute missing values up to n using the recurrence */
    if (n > last_idx) {
        for (idx_t i = last_idx + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + ((seq_t)16 * cache[i-2]);
        }
        last_idx = n;
    }

    return cache[n];
}
