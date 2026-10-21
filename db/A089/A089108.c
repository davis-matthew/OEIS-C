/*
 * A089108  Convoluted convolved Fibonacci numbers G_4^(r).
 * https://oeis.org/A089108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=5, a(3)=7, a(4)=10, a(5)=13, a(6)=16 a(n) =
 *            2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) for n ≥ 6
 *            (derived from the rational generating function, see OEIS
 *            A089108)
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index stored (≥0) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 8;                           /* initial capacity (≥6) */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* unlikely on failure */

        /* the sequence is 1‑based; cache[0] is unused */
        cache[0] = (seq_t)0;                     /* unused slot */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)10;
        cache[5] = (seq_t)13;
        /* a(6) can be computed by the recurrence or stored directly */
        cache[6] = (seq_t)16;
        cache_len = 6;                           /* we have values up to index 6 */
    }

    if (n <= cache_len) {
        return cache[n];
    }

    /* enlarge cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t v = 2 * cache[k - 1];
        v -= cache[k - 2];
        v += cache[k - 3];
        v -= 2 * cache[k - 4];
        v += cache[k - 5];
        cache[k] = v;
    }

    cache_len = n;
    return cache[n];
}
