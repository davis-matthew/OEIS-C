/*
 * A137243  Number of coprime pairs (a,b) with -n <= a,b <= n.
 * https://oeis.org/A137243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137243: Number of coprime pairs (a,b) with -n <= a,b <= n.
   Recurrence: a(1) = 8, a(n) = a(n-1) + 8*phi(n) for n >= 2.
   phi(m) is Euler's totient function.
*/

#include <stdlib.h>
#include "../utils/phi.h"

seq_t a(idx_t n) {
    /* Memoisation: keep a cache of already computed a(k) values. */
    static seq_t *cache = NULL;   /* cache[i] holds a(i+1), i >= 0 */
    static size_t cache_size = 0; /* number of values stored in cache */

    if (n == 0) return 0; /* undefined, but guard against underflow */

    /* Ensure the cache can hold a(n) */
    if (n > (idx_t)cache_size) {
        size_t required = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_cache) { /* allocation failure: abort */
            /* In a real project you might handle this differently */
            __builtin_trap();
        }
        cache = new_cache;

        /* Initialise or extend the cache */
        if (cache_size == 0) {
            /* First entry corresponds to a(1)=8 */
            cache[0] = (seq_t)8;
            cache_size = 1;
        }

        /* Compute new values using the recurrence */
        for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
            seq_t step = phi(i);          /* phi(i) */
            step *= (seq_t)8;              /* 8 * phi(i) */
            cache[i - 1] = cache[i - 2] + step;
        }
        cache_size = required;
    }

    return cache[n - 1];
}
