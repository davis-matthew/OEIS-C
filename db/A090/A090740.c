/*
 * A090740  Exponent of 2 in 3^n - 1.
 * https://oeis.org/A090740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = exponent of 2 in (3^n – 1). Recurrence relations (offset
 *            1): a(1) = 1 if n is odd → a(n) = 1 if n is even? → let k = n/2;
 *            a(n) = a(k) + 1 + [k odd]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard C header for memory allocation */
#include <stdlib.h>

/* Return the n-th term of A090740 (n ≥ 1).  Uses a lazy static cache. */
seq_t a(idx_t n)
{
    /* defined only for n ≥ 1; n == 0 is unused but handled gracefully */
    if (n == 0) return 0;

    /* static cache: cache[0] is unused, values start at index 1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of valid entries in cache */

    /* Expand the cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                 /* we need indices 0 .. n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                (size_t)new_size * sizeof(seq_t));
        if (!new_cache) return 0;                /* allocation failed – abort */

        /* Compute new values only; previous ones remain unchanged */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                new_cache[i] = 0;               /* placeholder, never used */
            } else if (i == 1) {
                new_cache[i] = 1;               /* a(1) = 1 */
            } else if ((i & 1) != 0) {
                new_cache[i] = 1;               /* odd indices → 1 */
            } else {
                idx_t k = i >> 1;
                new_cache[i] = new_cache[k] + 1 + ((k & 1) ? 1 : 0);
            }
        }

        /* Attach the newly sized cache back to the static variable */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
