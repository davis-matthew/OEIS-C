/*
 * A121798  a(n)= 4*a(n-1) +13*a(n-2) -44*a(n-3) -57*a(n-4) +120*a(n-5)
 *          +63*a(n-6) -56*a(n-7) +6*a(n-8).
 * https://oeis.org/A121798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 13*a(n-2) - 44*a(n-3) - 57*a(n-4) + 120*a(n-5)
 *            + 63*a(n-6) - 56*a(n-7) + 6*a(n-8) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache          = NULL;   /* dynamic cache for computed terms   */
static size_t cache_capacity = 0;      /* allocated size of the cache (>= n+1)  */
static idx_t  computed_max   = 0;      /* highest n that has been computed */

/* Base values for n = 1 .. 8.  Index 0 is unused. */
static const seq_t init[9] = {
    0,          /* dummy 0 index */
    0,          /* a(1) */
    28,         /* a(2) */
    408,        /* a(3) */
    1502,       /* a(4) */
    7821,       /* a(5) */
    31911,      /* a(6) */
    145162,     /* a(7) */
    616196      /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 – return 0 */

    /* Allocate or grow cache if necessary */
    if (cache_capacity < (size_t)(n + 1)) {
        size_t new_capacity = cache_capacity ? cache_capacity * 2 : 16;
        if (new_capacity < (size_t)(n + 1))
            new_capacity = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* Initialise missing base terms */
    if (computed_max < 8) {
        idx_t first = computed_max + 1;
        idx_t i;
        for (i = first; i <= 8 && i <= n; ++i)
            cache[i] = init[i];
    }

    /* Compute any remaining terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 8) /* already set above */ continue;

        /* Compute using positive and negative parts to avoid
           unsigned wrap‑around in intermediate results. */
        seq_t pos = 4 * cache[i-1] + 13 * cache[i-2]
                  + 120 * cache[i-5] + 63 * cache[i-6]
                  + 6   * cache[i-8];
        seq_t neg = 44 * cache[i-3] + 57 * cache[i-4] + 56 * cache[i-7];
        cache[i] = pos - neg;
    }

    computed_max = n;               /* all terms up to n are now cached   */
    return cache[n];
}
