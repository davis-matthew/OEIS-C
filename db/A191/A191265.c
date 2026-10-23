/*
 * A191265  Fixed point of the morphism 0 -> 001, 1 -> 002, 2 -> 01.
 * https://oeis.org/A191265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191265: fixed point of the morphism
   0 → 001, 1 → 002, 2 → 01
   (offset 1; a(1) = 0, a(2) = 0, …) */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached word */
static idx_t cache_len = 0;   /* length of cache in symbols */

/* Allocate an array of seq_t.  Use C++ new when compiled as C++ */
static seq_t *alloc_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Deallocate an array of seq_t */
static void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Grow the cache until it contains at least target symbols. */
static void grow_to(idx_t target)
{
    if (cache_len >= target)
        return;

    /* initialize with the first symbol 0 if cache is empty */
    if (cache_len == 0) {
        cache_len = 1;
        cache = alloc_array(1);
        cache[0] = 0;
    }

    while (cache_len < target) {
        /* compute length of the next expansion */
        idx_t i, next_len = 0;
        for (i = 0; i < cache_len; ++i) {
            if (cache[i] == 0 || cache[i] == 1)
                next_len += 3;
            else /* cache[i] == 2 */
                next_len += 2;
        }

        seq_t *new_cache = alloc_array((size_t)next_len);
        idx_t pos = 0;
        for (i = 0; i < cache_len; ++i) {
            seq_t c = cache[i];
            if (c == 0) {
                new_cache[pos++] = 0;
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
            } else if (c == 1) {
                new_cache[pos++] = 0;
                new_cache[pos++] = 0;
                new_cache[pos++] = 2;
            } else {            /* c == 2 */
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
            }
        }

        free_array(cache);
        cache = new_cache;
        cache_len = next_len;
    }
}

/* Return a(n) for n >= 1.  Undefined for n == 0; return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    grow_to(n);
    return cache[n - 1];
}
