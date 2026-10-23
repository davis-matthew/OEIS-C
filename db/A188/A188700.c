/*
 * A188700  Number of n X 3 binary arrays without the pattern 0 0 diagonally
 *          or vertically.
 * https://oeis.org/A188700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-2) + 6*a(n-3) - a(n-4) - a(n-5); Initial
 *            values: a(1)=8, a(2)=21, a(3)=90, a(4)=304, a(5)=1141
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values, 1‑based */
static idx_t cache_last = 0;     /* highest index computed so far */
static idx_t cache_capacity = 0; /* current allocated size (max idx) */

/* Base values for a(1) … a(5) */
static const seq_t base_vals[6] = {0, 8, 21, 90, 304, 1141};

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, values for n<1 are irrelevant */

    /* grow cache if necessary */
    if (n > cache_capacity) {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) return 0;    /* allocation failed – bad build, but safe */
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* iterate from the next uncomputed index up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 5) {
            /* Known initial values */
            cache[i] = base_vals[i];
        } else {
            /* Recurrence a(i) = a(i-1) + 8*a(i-2) + 6*a(i-3) - a(i-4) - a(i-5) */
            cache[i] = cache[i - 1]
                     + (seq_t)8 * cache[i - 2]
                     + (seq_t)6 * cache[i - 3]
                     - cache[i - 4]
                     - cache[i - 5];
        }
    }
    cache_last = n;
    return cache[n];
}
