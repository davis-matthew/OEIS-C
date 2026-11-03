/*
 * A013733  a(n) = 3^(3n+2).
 * https://oeis.org/A013733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(3n+2) * Recurrence: a(0) = 9, a(n) = 27 * a(n-1) (n >
 *            0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 188 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
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

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* pointer to stored terms      */
    static size_t sz   = 0;         /* number of terms stored       */
    static size_t cap  = 0;         /* capacity of the allocated array */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = alloc_seq(1);
        if (cache == NULL)          /* allocation failure */
            return 0;
        cache[0] = 9;               /* a(0) = 3^2 = 9 */
        sz   = 1;
        cap  = 1;
    }

    /* Return cached value if already computed */
    if ((size_t)n < sz)
        return cache[(size_t)n];

    /* Grow the array if necessary */
    if ((size_t)n + 1 > cap)
        grow_seq_array(&cache, &cap, (size_t)n + 1);

    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)sz; i <= n; ++i)
        cache[i] = cache[i - 1] * 27;

    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
