/*
 * A207456  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 13*a(n-2) + 4*a(n-3) - 16*a(n-4) for n >= 5
 *            Initial values: a(1)=10, a(2)=100, a(3)=270, a(4)=1450
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard library for memory allocation */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t computed_max = 0;

    /* Handle the special case where n == 0 (should not happen) */
    if (n == 0) {
        return 0;
    }

    /* Ensure cache is large enough to hold at least 4 elements */
    if (capacity == 0) {
        capacity = 8;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failure */
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)100;
        cache[3] = (seq_t)270;
        cache[4] = (seq_t)1450;
        computed_max = 4;
    }

    /* If we need more space, reallocate */
    if ((size_t)n >= capacity) {
        size_t newcap = capacity;
        while ((size_t)n >= newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;          /* allocation failure */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute values up to n if not already done */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            /* All indices are in range because cache is large enough */
            cache[i] = cache[i - 1] + (seq_t)13 * cache[i - 2]
                      + (seq_t)4  * cache[i - 3]
                      - (seq_t)16 * cache[i - 4];
        }
        computed_max = n;
    }

    return cache[n];
}
