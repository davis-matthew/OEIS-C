/*
 * A073952  Number of strings over Z_3 of length n with trace 1 and subtrace
 *          2.
 * https://oeis.org/A073952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) -15*a(n-2) +27*a(n-3) -36*a(n-4) +27*a(n-5) (for
 *            n≥6, initial values a(1)=0, a(2)=0, a(3)=3, a(4)=12, a(5)=30)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms */
    static seq_t *cache = NULL;          /* cache[i] holds a(i)   */
    static size_t cache_size = 0;        /* highest index computed */
    static size_t cache_cap   = 0;       /* allocated capacity     */

    /* Helper to extend cache up to needed index */
    if (cache == NULL) {
        /* Start with enough space for the first 5 terms */
        cache_cap = 6; /* indices 0..5 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory: return 0 */
        /* Initialize unused 0th element */
        cache[0] = 0;
        /* Base values */
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 3;
        cache[4] = 12;
        cache[5] = 30;
        cache_size = 5;
    }

    /* Resize if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* Use 128-bit arithmetic to avoid overflow for 64-bit builds */
            __int128 v =
                ( (__int128)6  * ( __int128)cache[i-1] )
              - ( (__int128)15 * ( __int128)cache[i-2] )
              + ( (__int128)27 * ( __int128)cache[i-3] )
              - ( (__int128)36 * ( __int128)cache[i-4] )
              + ( (__int128)27 * ( __int128)cache[i-5] );
            cache[i] = (seq_t)v;
        }
        cache_size = n;
    }

    return cache[n];
}
