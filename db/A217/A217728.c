/*
 * A217728  a(n) = a(n-1)^3 + a(n-2)^3 - (a(n-1) - a(n-2))^3, a(1) = a(2) = 1.
 * https://oeis.org/A217728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^3 + a(n-2)^3 - (a(n-1)-a(n-2))^3, a(1) = a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cached sequence values */
static size_t capacity = 0;       /* current allocated size of cache */
static idx_t len = 0;             /* number of values actually computed */

/* Ensure the cache has space for at least n elements and initialize
   the first two terms if necessary. */
static void ensure_cache(idx_t n)
{
    if (capacity < (size_t)n) {
        size_t new_cap = capacity ? capacity * 2 : 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        capacity = new_cap;
    }
    if (len < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        len = 2;
    }
}

seq_t a(idx_t n)
{
    if (n <= 2)  /* base cases */
        return (seq_t)1;

    ensure_cache(n);

    /* Compute missing terms up to n */
    for (idx_t i = len; i < n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t diff = t1 - t2;

        seq_t t1c = t1 * t1;
        t1c *= t1;         /* t1^3 */

        seq_t t2c = t2 * t2;
        t2c *= t2;         /* t2^3 */

        seq_t diffc = diff * diff;
        diffc *= diff;     /* diff^3 */

        cache[i] = t1c + t2c - diffc;
    }
    len = n;                /* all terms up to n are now computed */
    return cache[n - 1];
}
