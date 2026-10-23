/*
 * A180752  Half the number of nX3 binary arrays with each element equal to at
 *          least two neighbors.
 * https://oeis.org/A180752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) - a(n-5) with initial values
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 59, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* 0‑based: cache[0] = a(1) */
static idx_t  cache_size = 0;          /* number of allocated slots       */
static idx_t  computed_up_to = 0;      /* largest n already computed      */

static const seq_t base[5] = { (seq_t)0, 1, 1, 2, 3 };

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1, return 0 */

    /* Direct answer for the first five terms. */
    if (n <= 5) return base[n - 1];

    /* Ensure the cache is large enough. */
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        cache = tmp;
        cache_size = n;
    }

    /* Initialize the first five terms if this is the first call or the
       cache was resized. */
    if (computed_up_to < 5) {
        for (idx_t i = 0; i < 5; ++i) cache[i] = base[i];
        computed_up_to = 5;
    }

    /* If the required term is already computed, return it. */
    if (computed_up_to >= n) return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* The recurrence is valid only for i >= 6, but we are guaranteed
           to be in that range because computed_up_to starts at 5. */
        seq_t term = (seq_t)2 * cache[i - 2]   /* 2*a(i-1) */
                 +     cache[i - 3]           /*   + a(i-2) */
                 -     cache[i - 4]           /*   - a(i-3) */
                 -     cache[i - 6];          /*   - a(i-5) */
        cache[i - 1] = term;
    }
    computed_up_to = n;
    return cache[n - 1];
}
