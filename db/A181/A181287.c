/*
 * A181287  Numbers of the form i*5^j-1 (i=1..4, j >= 0).
 * https://oeis.org/A181287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-4) - 5*a(n-5) (recurrence valid for n ≥ 6)
 *            with initial values a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 3, a(5)
 *            = 4
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;        /* cache of computed terms (0‑based) */
static size_t cache_len = 0;       /* number of terms stored in cache */
static size_t cache_cap = 0;       /* allocated capacity of the cache */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* offset is 1; behaviour below offset is irrelevant */

    /* Convert the uint64_t index to size_t for array indexing. */
    size_t idx = (size_t)n;

    /* Allocate or grow the cache if necessary. */
    if (idx > cache_cap) {
        size_t new_cap = idx;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();               /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill any missing terms up to n (0‑based index up to idx‑1). */
    for (size_t i = cache_len; i < idx; ++i) {   /* i: 0‑based index, n = i+1 */
        seq_t val;
        size_t term = i + 1;
        if (term == 1)
            val = 0;
        else if (term == 2)
            val = 1;
        else if (term == 3)
            val = 2;
        else if (term == 4)
            val = 3;
        else if (term == 5)
            val = 4;
        else /* term >= 6: use recurrence */
            val = cache[i-1] + 5 * cache[i-4] - 5 * cache[i-5];

        cache[i] = val;
    }
    cache_len = idx;

    return cache[idx-1];
}
