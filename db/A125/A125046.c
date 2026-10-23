/*
 * A125046  Partial sums of A003095.
 * https://oeis.org/A125046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = a(n-1)^2 - 2*a(n-1)*a(n-2) + a(n-2)^2
 *            + a(n-1) + 1 for n > 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* dynamically expanded array */
    static size_t cache_cap = 0;     /* current capacity of cache */
    static idx_t cached_upto = 0;    /* largest index computed so far */

    /* Expand cache capacity if needed */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two values if this is the first call */
    if (cached_upto == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cached_upto = 1;
    }

    /* Compute missing terms up to n */
    if (cached_upto < n) {
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t term = a1 * a1
                       - (seq_t)2 * a1 * a2
                       + a2 * a2
                       + a1
                       + (seq_t)1;
            cache[i] = term;
        }
        cached_upto = n;
    }

    return cache[n];
}
