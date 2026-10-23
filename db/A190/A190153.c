/*
 * A190153  Row sums of the triangle A190152.
 * https://oeis.org/A190153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2) + a(n-3) with a(0)=1, a(1)=2, a(2)=12
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamic array of computed terms */
static size_t  cache_len = 0;      /* number of terms currently stored (max index+1) */
static size_t  cache_cap = 0;      /* allocated size of cache */

/* ensure the cache has room for at least `need` elements */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* OEIS sequence A190153 */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* convert to non‑negative size_t */

    /* handle the first three terms directly */
    if (idx == 0) return (seq_t)1;
    if (idx == 1) return (seq_t)2;
    if (idx == 2) return (seq_t)12;

    /* ensure we have room for the result and initialize the first three terms if necessary */
    ensure_cache_cap(idx + 1);
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)12;
        cache_len = 3;
    }

    /* compute missing terms up to idx */
    if (cache_len <= idx) {
        size_t i;
        for (i = cache_len; i <= idx; ++i) {
            cache[i] = (seq_t)(5 * cache[i-1] + 2 * cache[i-2] + cache[i-3]);
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
