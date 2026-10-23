/*
 * A144476  Four interlaced copies of A117373.
 * https://oeis.org/A144476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-4) - a(n-8) for n ≥ 9 a(1)= 1, a(2)= 1, a(3)= -1,
 *            a(4)= 3, a(5)= -2, a(6)= -2, a(7)= 2, a(8)= 1 (indices start at
 *            1) The sequence is computed with memoisation and a simple
 *            iterative recurrence
 * Range:     i8 n <= 51, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache, largest index computed so far, and capacity */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_comp = 0;

    /* n < 1 is outside the offset; return 0 */
    if (n < 1) return 0;

    /* Allocate or grow the cache if needed */
    if (cache == NULL || cache_cap <= n) {
        /* allocate at least n+1 elements, doubled for future growth */
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* unrecoverable allocation failure */
        cache = tmp;
        if (cache_cap == 0) {
            cache[0] = 0;            /* index 0 is unused, keep safe */
        }
        cache_cap = new_cap;
    }

    /* If requested value is already computed, return it */
    if (n <= max_comp)
        return cache[n];

    /* Initialise first 8 terms the first time or when required */
    if (max_comp < 8) {
        static const seq_t init[9] = {
            0, /* unused */
            1, 1, -1, 3, -2, -2, 2, 1
        };
        for (idx_t i = 1; i <= 8 && i <= n; ++i)
            cache[i] = init[i];
        max_comp = (n < 8 ? n : 8);
    }

    /* Compute missing terms using the recurrence a(n)=a(n-4)-a(n-8) */
    if (n > max_comp) {
        /* start from the next index, but the recurrence only holds for n≥9 */
        idx_t start = (max_comp < 9 ? 9 : max_comp + 1);
        for (idx_t i = start; i <= n; ++i)
            cache[i] = cache[i - 4] - cache[i - 8];
        max_comp = n;
    }

    return cache[n];
}
