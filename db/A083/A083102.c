/*
 * A083102  a(n) = 2*a(n-1) + 10*a(n-2), with a(0) = 1, a(1) = 2.
 * https://oeis.org/A083102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2), a(0)=1, a(1)=2 The function
 *            memoises already-computed values so that subsequent calls with
 *            arbitrary indices are answered in O(1); the first call that
 *            reaches an index uses an O(n) loop to fill the table up to that
 *            index. Arithmetic is performed on type seq_t as required
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of filled entries */
    static idx_t cache_cap = 0;      /* allocated size */

    /* expand cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 8) new_cap = 8;                /* minimum size */
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc may return NULL, but that indicates OOM in this context;
           we simply return 0 to keep the solution simple. */
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill cache up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)2;
        else
            cache[i] = (2 * cache[i - 1]) + (10 * cache[i - 2]);
    }
    cache_len = n + 1;

    return cache[n];
}
