/*
 * A025921  Expansion of 1/((1-x^8)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A025921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 8, 10, and 11.
 *            Recurrence: a(n) = a(n-8) + a(n-10) + a(n-11) - a(n-18) -
 *            a(n-19) - a(n-21) + a(n-29) Initial values: a(0) = 1 a(k) = 0
 *            for 1 ≤ k < 8
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_last = 0;

    /* Expand cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = NULL;

#ifdef __cplusplus
        /* Allocate and value‑initialize (zero) for class types */
        new_cache = new seq_t[new_size]();
#else
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed */
        for (idx_t i = 0; i < new_size; ++i) new_cache[i] = 0;
#endif

        if (cache) {
            /* Copy existing values */
            for (idx_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];

#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }

        cache = new_cache;
        cache_size = new_size;

        /* Initialise the only known value */
        if (computed_last == 0) {
            cache[0] = 1;
            computed_last = 0;
        }
    }

    /* Compute new terms up to n */
    for (idx_t i = computed_last + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 8)  val += cache[i - 8];
        if (i >= 10) val += cache[i - 10];
        if (i >= 11) val += cache[i - 11];
        if (i >= 18) val -= cache[i - 18];
        if (i >= 19) val -= cache[i - 19];
        if (i >= 21) val -= cache[i - 21];
        if (i >= 29) val += cache[i - 29];
        cache[i] = val;
    }

    computed_last = n;
    return cache[n];
}
