/*
 * A238374  Row sums of triangle in A204026.
 * https://oeis.org/A238374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5), with initial
 *            values a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 6, a(4) = 9
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc */

seq_t a(idx_t n)
{
    /* Initial values for the first five terms */
    static const seq_t init_vals[5] = { 1, 2, 4, 6, 9 };

    /* Static storage for memoisation */
    static seq_t *cache        = NULL;  /* pointer to allocated array of values */
    static size_t  cap         = 0;     /* current allocated size (in elements) */
    static size_t  computed    = (size_t)-1; /* highest index that has been computed */

    /* Return early for the first five terms */
    if (n < 5)
        return init_vals[n];

    /* If cache not initialised yet, allocate a small initial block */
    if (cap == 0) {
        cap = 16;
        /* Use realloc with an explicit cast so the code compiles in C++ as well */
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) /* allocation failure – fallback to zeroed value */
            return 0;
        cache = tmp;

        /* Store the initial five values in the cache and set computed */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
        computed = 4;  /* the highest index computed so far */
    }

    /* Grow the cache if necessary to accommodate index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;   /* double until we can hold n */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;       /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n using the recurrence */
    if (computed < (size_t)n) {
        for (size_t i = computed + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2];
            cache[i] -= cache[i - 3];
            cache[i] += cache[i - 4];
            cache[i] -= cache[i - 5];
        }
        computed = (size_t)n;
    }

    return cache[(size_t)n];
}
