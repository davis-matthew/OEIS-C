/*
 * A048335  a(n) in base 11 is a repdigit.
 * https://oeis.org/A048335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n - 10*floor((n-1)/10))*(11^floor((n+9)/10) - 1)/10
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 91, u64 n <= 183,
 *            u128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n > 19: a(n) = 12*a(n-10) - 11*a(n-20) */
/* Initial values (a(0) … a(19)) are listed explicitly. */

#include <stdlib.h>

static const seq_t init_vals[20] = {
    0ULL, 1ULL, 2ULL, 3ULL, 4ULL, 5ULL, 6ULL, 7ULL, 8ULL, 9ULL,
    10ULL, 12ULL, 24ULL, 36ULL, 48ULL, 60ULL, 72ULL, 84ULL, 96ULL, 108ULL
};

static seq_t *cache = NULL;      /* dynamic array holding computed terms */
static idx_t capacity = 0;       /* allocated capacity */
static idx_t computed = 0;       /* highest index computed so far (>=0) */

seq_t a(idx_t n)
{
    /* Ensure the cache has enough space. */
    if (n >= capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialize cache on first call. */
    if (computed == 0) {
        for (idx_t i = 0; i < 20 && i < capacity; ++i)
            cache[i] = init_vals[i];
        computed = (capacity > 19) ? 19 : (idx_t)(capacity - 1);
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 19) {
            cache[i] = init_vals[i];
        } else {
            /* a(i) = 12*a(i-10) - 11*a(i-20) */
            cache[i] = (seq_t)((seq_t)12 * cache[i - 10] -
                               (seq_t)11 * cache[i - 20]);
        }
    }
    computed = n;
    return cache[n];
}
