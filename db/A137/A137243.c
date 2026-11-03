/*
 * A137243  Number of coprime pairs (a,b) with -n <= a,b <= n.
 * https://oeis.org/A137243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9674 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137243: Number of coprime pairs (a,b) with -n <= a,b <= n.
   Recurrence: a(1) = 8, a(n) = a(n-1) + 8*phi(n) for n >= 2.
   phi(m) is Euler's totient function.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Memoisation: keep a cache of already computed a(k) values. */
    static seq_t *cache = NULL;   /* cache[i] holds a(i+1), i >= 0 */
    static size_t cache_size = 0; /* number of values stored in cache */

    if (n == 0) return 0; /* undefined, but guard against underflow */

    /* Ensure the cache can hold a(n) */
    if (n > (idx_t)cache_size) {
        size_t required = (size_t)n;
        seq_t *new_cache = mem_resize(seq_t, cache, required);
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
            seq_t step = totient(i);          /* phi(i) */
            step *= (seq_t)8;              /* 8 * phi(i) */
            cache[i - 1] = cache[i - 2] + step;
        }
        cache_size = required;
    }

    return cache[n - 1];
}
