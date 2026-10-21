/*
 * A218837  Unmatched value maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..1 n X 3 array.
 * https://oeis.org/A218837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2) + a(n-3); a(1)=4, a(2)=20, a(3)=93
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache       = NULL;
    static size_t cache_cap   = 0;
    static idx_t  computed_max = 0;

    /* Initial allocation and base values */
    if (cache == NULL) {
        cache_cap = 4; /* indices 0..3, index 0 unused */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            abort(); /* allocation failure */

        cache[1] = (seq_t)4;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)93;
        computed_max = 3;
    }

    /* If already computed, return the stored value */
    if (n <= computed_max)
        return cache[(size_t)n];

    /* Ensure the array is large enough */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from computed_max+1 up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)4 * cache[(size_t)(i - 1)] +
            (seq_t)3 * cache[(size_t)(i - 2)] +
              cache[(size_t)(i - 3)];
    }

    computed_max = n;
    return cache[(size_t)n];
}
