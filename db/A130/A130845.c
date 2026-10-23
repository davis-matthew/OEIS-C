/*
 * A130845  a(4n) = a(4n+1) = a(4n+2) = A001477(n), a(4n+3) = A005408(n).
 * https://oeis.org/A130845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=0, a(3)=1, a(4)=a(5)=a(6)=a(7)=1, a(7)=3
 *            Recurrence: a(n) = 2*a(n-4) - a(n-8) for n >= 8 This is
 *            equivalent to the definition: a(4k) = a(4k+1) = a(4k+2) =
 *            A001477(k), a(4k+3) = A005408(k)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static const seq_t init_vals[8] = {0, 0, 0, 1, 1, 1, 1, 3};

static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* Return small pre‑known values directly. */
    if (n < 8u) return init_vals[(size_t)n];

    /* Ensure the cache contains all values up to n. */
    if (cache_size == 0) {
        /* Allocate space for at least 8 elements. */
        size_t sz = 8;
        seq_t *new_cache = (seq_t *)malloc(sz * sizeof(seq_t));
        assert(new_cache != NULL);
        cache = new_cache;
        cache_size = sz;
        /* Initialize the first 8 values. */
        for (size_t i = 0; i < 8; ++i) cache[i] = init_vals[i];
    }

    /* If n is already inside the cache, just return it. */
    if (n < cache_size) return cache[(size_t)n];

    /* Grow the cache to accommodate n. */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        assert(new_cache != NULL);
        cache = new_cache;
        /* Compute missing values from old_size to n. */
        for (size_t i = cache_size; i < new_size; ++i) {
            /* Recurrence: a(i) = 2*a(i-4) - a(i-8) */
            seq_t v1 = cache[i - 4];
            seq_t v2 = cache[i - 8];
            cache[i] = v1 + v1 - v2;  /* 2 * v1 - v2 */
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
