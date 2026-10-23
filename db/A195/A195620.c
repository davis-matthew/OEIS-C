/*
 * A195620  Numerators of Pythagorean approximations to 4.
 * https://oeis.org/A195620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 65*a(n-1) + 65*a(n-2) - a(n-3); with initial values
 *            a(1)=63, a(2)=4161, a(3)=274559
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Allow direct return of the first three terms without cache usage */
    if (n == 1) return (seq_t)63;
    if (n == 2) return (seq_t)4161;
    if (n == 3) return (seq_t)274559;
    if (n < 1)   /* undefined offset – return zero */
        return 0;

    static seq_t *cache = NULL;   /* 1‑based cache, cache[0] unused */
    static idx_t computed = 0;    /* highest index already computed */
    static idx_t capacity = 0;    /* size of allocated cache */

    /* Ensure the cache is large enough for indices up to n */
    if (capacity < 4) {
        capacity = 4;                          /* allocate at least 4 entries */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        /* initialise the known values */
        cache[1] = (seq_t)63;
        cache[2] = (seq_t)4161;
        cache[3] = (seq_t)274559;
        computed = 3;
    }

    /* Expand the cache if necessary */
    if (capacity <= n) {
        idx_t new_cap = capacity * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute terms up to n, using the recurrence relation */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* a(i) = 65*a(i-1) + 65*a(i-2) - a(i-3) */
        cache[i] = (seq_t)65 * cache[i - 1] +
                   (seq_t)65 * cache[i - 2] -
                   cache[i - 3];
    }
    computed = n;

    return cache[n];
}
