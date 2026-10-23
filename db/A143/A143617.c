/*
 * A143617  Where record values occur in A010371.
 * https://oeis.org/A143617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A143617: Where record values occur in A010371
   Formula: a(n) = 10*a(n-7) + 8   for n > 11
   Initial values (n = 1…11):
     a(1) = 0,  a(2) = 8,  a(3) = 10,  a(4) = 18,  a(5) = 20,
     a(6) = 28, a(7) = 68, a(8) = 88, a(9) = 108, a(10) = 188, a(11) = 200.
   The sequence is computed on demand and cached; any call order is allowed.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;   /* 1‑based array; index 0 unused */
    static idx_t cached_len = 0;  /* highest index stored in cache */

    /* if the cache is not yet initialized, create it with the first 11 terms */
    if (cached_len == 0) {
        const size_t init_len = 12;          /* 0..11, we ignore index 0 */
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure – unlikely in tests */

        /* assign known values */
        cache[1] = 0;
        cache[2] = 8;
        cache[3] = 10;
        cache[4] = 18;
        cache[5] = 20;
        cache[6] = 28;
        cache[7] = 68;
        cache[8] = 88;
        cache[9] = 108;
        cache[10] = 188;
        cache[11] = 200;
        cached_len = 11;
    }

    if (n <= cached_len) return cache[n];

    /* extend the cache to hold the requested index */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;                 /* allocation failure – unlikely */

    cache = new_cache;

    /* fill in the missing terms using the recurrence a(k)=10*a(k-7)+8 */
    for (idx_t k = cached_len + 1; k <= n; ++k) {
        cache[k] = 10 * cache[k - 7] + 8;
    }

    cached_len = n;
    return cache[n];
}
