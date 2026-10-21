/*
 * A224669  Number of (n+1) X 2 0..2 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=12, a(2)=25, a(3)=41; for n≥4: a(n)=2*a(n-1)-a(n-3)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* buffer for memoised values */
static size_t last_computed = 0;   /* highest index that has been filled */
static size_t cache_capacity = 0;  /* current buffer size */

/* Return the n-th term of A224669.  n is 1‑based; a(1) = 12. */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below the offset is irrelevant */
        return 0;

    /* we use 1‑based indexing; index 0 stays unused */
    size_t need = (size_t)n + 1;

    /* grow the buffer if required */
    if (need > cache_capacity) {
        size_t new_cap = cache_capacity ? (cache_capacity << 1) : 8;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)   /* allocation failed */
            return 0;
        cache = tmp;
        for (size_t i = cache_capacity; i < new_cap; ++i)
            cache[i] = 0;
        cache_capacity = new_cap;
    }

    /* initialise the first three values if not already done */
    if (last_computed < 3) {
        cache[1] = 12;   /* a(1) */
        cache[2] = 25;   /* a(2) */
        cache[3] = 41;   /* a(3) */
        last_computed = 3;
    }

    /* compute any missing terms up to n */
    for (size_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] - cache[i - 3];
    }
    if (n > last_computed)
        last_computed = (size_t)n;

    return cache[n];
}
