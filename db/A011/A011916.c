/*
 * A011916  a(n) = ((b(n)-1)+sqrt(3*b(n)^2-4*b(n)+1))/2, where b(n) is
 *          A011922(n).
 * https://oeis.org/A011916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1)-15*a(n-2)+a(n-3), a(0)=0, a(1)=3, a(2)=44
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 869 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* highest index computed (initially 0 indices pre‑filled) */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache = alloc_seq_array(3);      /* allocate 3 elements */
        cache_cap = 3;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)44;
        cache_len = 2;   /* indices 0,1,2 are known */
    }

    /* if the requested value is already computed, return it */
    if ((size_t)n <= cache_len)
        return cache[n];

    /* grow the array so it can hold up to index n */
    if ((size_t)n + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)15 * cache[i - 1]
                 - (seq_t)15 * cache[i - 2]
                 + cache[i - 3];
    }

    cache_len = (size_t)n;
    return cache[n];
}
