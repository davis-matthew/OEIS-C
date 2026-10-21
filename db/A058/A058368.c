/*
 * A058368  Number of ways to cover (without overlapping) a ring lattice
 *          (necklace) of n sites with molecules that are 5 sites wide.
 * https://oeis.org/A058368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5); Initial values (n ≥ 1): a(1) = a(2) =
 *            a(3) = a(4) = 1 a(5) = 6 For n ≥ 6 the recurrence holds
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 is assumed; return 0 if called with 0 for safety */
    if (n == 0) return 0;

    /* static cache for previously computed values */
    static seq_t *cache   = NULL;
    static size_t capacity = 0;   /* allocated length (in seq_t elements) */
    static idx_t  last     = 0;   /* highest index that has been computed */

    /* Ensure the cache is initialized with the first five terms. */
    if (cache == NULL) {
        /* At least indices 1..5 must be valid, extra space for growth. */
        capacity = 16;
        if (capacity < (size_t)6) capacity = 6;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;            /* allocation failed (unlikely) */
        /* set base values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)6;
        last = 5;
    }

    /* Grow the cache if the requested index exceeds current capacity. */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity ? capacity << 1 : 16;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* reallocation failed */
        cache   = tmp;
        capacity = new_cap;
    }

    /* If the value has not yet been computed, extend the cache using the recurrence. */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 5) {
                /* Base cases already set; skip. */
                continue;
            }
            cache[(size_t)i] = cache[(size_t)(i - 1)] + cache[(size_t)(i - 5)];
        }
        last = n;
    }

    return cache[(size_t)n];
}
