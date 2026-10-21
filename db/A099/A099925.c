/*
 * A099925  a(n) = Lucas(n) + (-1)^n.
 * https://oeis.org/A099925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) + (-1)^n Recurrence used (order 3): a(0) = 3,
 *            a(1) = 0, a(2) = 4 a(n) = 2*a(n-2) + a(n-3) for n >= 3 The
 *            implementation caches already computed values so that any order
 *            of calls is efficient
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored terms a[0..max_cached]   */
    static idx_t  max_cached = 0;    /* highest index that has a value  */
    static idx_t  cache_len  = 0;    /* allocated size of cache          */

    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* initial cache setup */
    if (cache == NULL) {
        cache_len = (idx_t) (3 > n ? 3 : n+1); /* ensure at least n+1 */
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, return 0 as sentinel */

        cache[0] = (seq_t)3;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        max_cached = 2;
    }

    /* enlarge cache if necessary */
    if (n > max_cached) {
        idx_t new_len = n + 1;
        if (new_len > cache_len) {
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cache_len = new_len;
        }
        /* compute missing terms using recurrence */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t term = (cache[i-2] << 1) + cache[i-3];
            cache[i] = term;
        }
        max_cached = n;
    }

    return cache[n];
}
