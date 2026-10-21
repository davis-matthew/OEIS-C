/*
 * A229386  Number of n X 2 0..3 arrays of the sum of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 3 0..1 array.
 * https://oeis.org/A229386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 36*a(n-2) + 32*a(n-3) - 16*a(n-4) (n≥5)
 *            Initial values: a(1)=14, a(2)=128, a(3)=1064, a(4)=8592
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed terms    */
static size_t cache_cap = 0;     /* current capacity of cache          */
static size_t cache_size = 0;    /* number of terms stored (≥0)        */

/* 1‑based initial terms, stored for quick first access */
static const seq_t init[4] = {14, 128, 1064, 8592};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* safety guard, offset is 1 */

    size_t idx = (size_t)n;              /* use 0‑based indexing in cache */

    /* Allocate or extend the cache if needed.  */
    if (cache_size < idx) {
        if (cache == NULL) {
            /* First allocation: ensure capacity for at least four terms. */
            cache_cap = (idx > 4) ? idx : 4;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();        /* OOM: abort the program */
            /* Copy the initial values. */
            for (size_t i = 0; i < 4 && i < cache_cap; ++i)
                cache[i] = init[i];
            cache_size = 4;
        } else if (cache_cap < idx) {
            /* Grow the cache, doubling capacity until it fits. */
            size_t new_cap = cache_cap * 2;
            if (new_cap < idx) new_cap = idx;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms up to the requested index using the recurrence. */
        while (cache_size < idx) {
            seq_t a1 = cache[cache_size - 1];
            seq_t a2 = cache[cache_size - 2];
            seq_t a3 = cache[cache_size - 3];
            seq_t a4 = cache[cache_size - 4];
            seq_t val = 12 * a1 - 36 * a2 + 32 * a3 - 16 * a4;
            cache[cache_size] = val;
            ++cache_size;
        }
    }

    return cache[idx - 1];
}
