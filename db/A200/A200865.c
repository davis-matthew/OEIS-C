/*
 * A200865  Number of 0..2 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors or less than both
 *          neighbors.
 * https://oeis.org/A200865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-4) Initial values: a(1)=17, a(2)=37,
 *            a(3)=77, a(4)=163 (valid for all n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 116,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 207 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

static seq_t *cache = NULL;        /* cache[0] unused, cache[1] holds a(1) */
static size_t cache_size = 0;      /* allocated array size in elements */
static idx_t  computed_max = 0;    /* largest n whose term has been computed */

seq_t a(idx_t n)
{
    /* sequence is defined for n ≥ 1 */
    if (n < 1)
        return (seq_t)0;

    /* ensure the cache can hold index n (cache[0] unused) */
    if ((size_t)n + 1 > cache_size)
        ensure_capacity((size_t)n + 1, &cache_size, (void **)&cache,
                         sizeof(seq_t));

    /* compute any missing terms up to n */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)17;
            else if (i == 2)
                cache[i] = (seq_t)37;
            else if (i == 3)
                cache[i] = (seq_t)77;
            else if (i == 4)
                cache[i] = (seq_t)163;
            else
                cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 4];
        }
        computed_max = n;
    }

    return cache[n];
}
