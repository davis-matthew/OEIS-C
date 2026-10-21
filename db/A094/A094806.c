/*
 * A094806  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 8 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 1, s(2n) = 5.
 * https://oeis.org/A094806
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3), a(2)=1, a(3)=5, a(4)=20,
 *            defined for all n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; return 0 safely. */
    if (n < 2) return 0;

    /* Static cache holding computed values 0 … cached_max. */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;   /* highest n for which cache[n] is valid */

    /* --- Initialization: allocate minimal cache (indices 0…4) --- */
    if (cache == NULL) {
        size_t init_size = 8;          /* ensure enough room for index 7 */
        cache = (seq_t *)calloc(init_size, sizeof(seq_t));
        /* Base values: a(2), a(3), a(4) */
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)20;
        cached_max = 4;
    }

    /* If the desired n is already cached, return it. */
    if (n <= cached_max) {
        return cache[n];
    }

    /* --- Expand cache so that it can hold indices up to n --- */
    size_t new_size = (size_t)(cached_max + 1);   /* current number of elements */
    /* Grow until we have room for index n */
    while (new_size <= (size_t)n) {
        new_size <<= 1;
    }
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* Memory allocation failed: return 0 (unlikely in test harness). */
        return 0;
    }
    cache = tmp;
    /* Zero‑initialize the newly added slots. */
    for (size_t i = cached_max + 1; i < new_size; ++i) {
        cache[i] = 0;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t term = (seq_t)6 * cache[i - 1]
                   - (seq_t)10 * cache[i - 2]
                   + (seq_t)4  * cache[i - 3];
        cache[i] = term;
    }

    /* Update cached_max and return the requested value. */
    cached_max = n;
    return cache[n];
}
