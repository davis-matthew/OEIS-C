/*
 * A116555  a(2*n+1) = 9*a(n), a(2*n+2) = 10*a(n) + a(n-1).
 * https://oeis.org/A116555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1 * a(2*n+1) = 9*a(n) * a(2*n+2) = 10*a(n)+a(n-1)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 1025, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Memoisation buffer: cache[i] contains a(i). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored */

    /* Ensure the buffer can hold all values up to n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)        /* out‑of‑memory: return 0 as a fallback */
            return 0;
        cache = new_cache;

        /* Initialise the very first element if it hasn't been set yet. */
        if (cache_size == 0) {
            cache[0] = 1;
            cache_size = 1;
        }

        /* Compute values that are not yet cached. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i & 1) {                       /* odd index: 2*k+1 */
                idx_t k = (i - 1) >> 1;        /* k = floor(i/2) */
                cache[i] = 9 * cache[k];
            } else {                           /* even index: 2*k */
                idx_t k = i >> 1;              /* k = i/2 */
                seq_t t1 = (k >= 1) ? cache[k - 1] : 0;
                seq_t t2 = (k >= 2) ? cache[k - 2] : 0;
                cache[i] = 10 * t1 + t2;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
