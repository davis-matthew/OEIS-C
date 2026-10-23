/*
 * A026532  Ratios of successive terms are 3, 2, 3, 2, 3, 2, 3, 2, ...
 * https://oeis.org/A026532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(5+(-1)^n)*a(n-1), a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = a(n-1) * (if n even then 3 else 2) */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache, index 0 unused */
static idx_t cache_size = 0;     /* largest n that has been computed */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */

    /* enlarge cache if necessary */
    if (n > cache_size) {
        /* realloc does the copying for us; allocate enough for index n */
        size_t new_cap = (size_t)(n + 1);   /* leave index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                /* allocation failed */

        cache = tmp;

        /* initialise base term if this is the first allocation */
        if (cache_size == 0)
            cache[1] = (seq_t)1;

        /* compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)        /* base case already set */
                continue;
            seq_t mult = (i & 1) ? (seq_t)2 : (seq_t)3;   /* odds → 2, evens → 3 */
            cache[i] = cache[i - 1] * mult;
        }

        cache_size = n;
    }

    return cache[n];
}
