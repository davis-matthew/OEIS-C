/*
 * A175014  y-values in the solution to x^2-55*y^2=1.
 * https://oeis.org/A175014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 178*a(n-1) - a(n-2), a(1) = 0, a(2) = 12
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache for memoization. Indexes are 1-based. */
    static seq_t *cache = NULL;
    static idx_t allocated = 0;   /* maximum usable index + 1 */
    static idx_t highest = 0;     /* largest n computed */

    /* Initialize cache on first call. */
    if (cache == NULL) {
        /* Ensure room for indices 0..2 (we ignore 0). */
        allocated = 4;
        cache = (seq_t *)calloc(allocated, sizeof(seq_t));
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        highest = 2;
    }

    /* Sanity check: if the requested n is less than or equal to
       the highest computed value, we can return immediately. */
    if (n <= highest)
        return cache[n];

    /* Grow the cache as necessary. */
    if (n >= allocated) {
        size_t new_alloc = allocated;
        if (new_alloc < 4)
            new_alloc = 4;
        while (new_alloc <= (size_t)n)
            new_alloc <<= 1;
        cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        allocated = (idx_t)new_alloc;
    }

    /* Compute values from highest+1 up to n. */
    for (idx_t i = highest + 1; i <= n; ++i) {
        cache[i] = 178 * cache[i - 1] - cache[i - 2];
    }
    highest = n;
    return cache[n];
}
