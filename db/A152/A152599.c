/*
 * A152599  a(n) = 10*a(n-1) - 12*a(n-2) for n > 1; a(0) = 1, a(1) = 4 .
 * https://oeis.org/A152599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS Sequence A152599
 *
 * Recurrence:   a(0) = 1
 *               a(1) = 4
 *               a(n) = 10*a(n-1) - 12*a(n-2)   for n > 1
 *
 * The implementation caches computed values so that an arbitrary
 * sequence of calls with any order in 0 … n will be handled correctly.
 * It uses only the facilities required by the build: <stdlib.h> for
 * malloc/realloc/exit, and the types seq_t and idx_t supplied by the
 * compiler.  No other utilities are required.
 */
#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed values   */
    static size_t  cap    = 0;      /* current capacity of cache  */
    static size_t  len    = 0;      /* number of values already computed */

    /* Initialise cache with the first two terms if it is empty */
    if (len == 0) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        len = 2;
    }

    /* If the requested index is already computed, return it */
    if ((size_t)n < len) {
        return cache[(size_t)n];
    }

    /* Ensure the cache is large enough to hold up to index n */
    size_t required = (size_t)n + 1;
    if (cap < required) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap < required) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to index n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)10 * prev1 - (seq_t)12 * prev2;
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
