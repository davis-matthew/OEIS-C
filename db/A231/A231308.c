/*
 * A231308  Recurrence a(n) = a(n-2) + n^M for M=9, starting with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A231308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum{k=0..floor(n/2)}(n-2k)^9 (OEIS A231308)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 112, u128 n <= 9621,
 *            big any n
 * Helpers:   pow_i9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_i9.h"

/*
 * Memoised recurrence:
 *   a(0) = 0
 *   a(1) = 1
 *   a(n) = a(n-2) + n^9   for n >= 2
 *
 * The cache grows as needed; computation is O(n) per new n.
 */
seq_t a(idx_t n) {
    /* Static cache (lazy allocation). */
    static seq_t *cache = NULL;        /* array of values a(0)..a(k) */
    static idx_t cache_len = 0;        /* number of values stored so far */
    static size_t cache_cap = 0;       /* allocated size of cache */

    /* Ensure there is room for at least n+1 entries. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 16)
            new_cap = 16;               /* grow with a minimum size */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                     /* allocation failure */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases if not yet present. */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = 1;
        cache_len = 2;
    }

    /* Compute missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i >= 2) {
            seq_t term = pow_i9((seq_t)i);
            val = cache[i - 2] + term;
        } else {
            /* i is 0 or 1 – handled by the initialisation above */
            val = cache[i];
        }
        cache[i] = val;
    }
    cache_len = n + 1;                    /* all values up to n are now cached */
    return cache[n];
}
