/*
 * A202400  Number of (n+2) X 4 binary arrays avoiding patterns 000 and 010 in
 *          rows and columns.
 * https://oeis.org/A202400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(n) = 6*a(n-1) - 6*a(n-2) + 30*a(n-3) - 30*a(n-5)
          + 6*a(n-6) - 6*a(n-7) + a(n-8)   (n≥9)
   Initial values:
   a(1)=441, a(2)=2401, a(3)=14161, a(4)=83521,
   a(5)=485809, a(6)=2825761, a(7)=16475481, a(8)=96059601 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialize cache with the first 8 terms */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */

        cache[1] = (seq_t)441;
        cache[2] = (seq_t)2401;
        cache[3] = (seq_t)14161;
        cache[4] = (seq_t)83521;
        cache[5] = (seq_t)485809;
        cache[6] = (seq_t)2825761;
        cache[7] = (seq_t)16475481;
        cache[8] = (seq_t)96059601;

        cache_len = 8;
    }

    if (n <= cache_len)
        return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = 6 * cache[i-1];
        val -= 6 * cache[i-2];
        val += 30 * cache[i-3];
        val -= 30 * cache[i-5];
        val += 6 * cache[i-6];
        val -= 6 * cache[i-7];
        val += cache[i-8];
        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
