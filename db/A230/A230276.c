/*
 * A230276  Voids left after packing 5-curves coins patterns into fountain of
 *          coins with base n.
 * https://oeis.org/A230276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230276
   Recurrence: a(1..6) = 0,1,1,6,10,16
   For n>=7: a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6)
 */
#include <stdlib.h>

static const seq_t init_vals[6] = {0, 1, 1, 6, 10, 16};

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* handle index below offset */
    if (n == 0) return 0;

    /* initialize cache once */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failed */
        for (size_t i = 0; i < 6; ++i) cache[i] = init_vals[i];
        cache_size = 6;
    }

    if ((size_t)n <= cache_size)
        return cache[n-1];          /* already computed */

    /* extend cache */
    size_t target = (size_t)n;
    if (target > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < target) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t idx = cache_size + 1; idx <= target; ++idx) {
        /* indices: cache[idx-1] for a(idx) */
        seq_t p = cache[idx-2] + cache[idx-3] + cache[idx-7];        /* a(n-1)+a(n-2)+a(n-6) */
        seq_t q = cache[idx-5] + cache[idx-6];                       /* a(n-4)+a(n-5)   */
        cache[idx-1] = p - q;
    }
    cache_size = target;
    return cache[n-1];
}
