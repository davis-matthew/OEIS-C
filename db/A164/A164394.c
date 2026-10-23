/*
 * A164394  Number of binary strings of length n with no substrings equal to
 *          0001 or 0100.
 * https://oeis.org/A164394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(n) = 2*a(n-1) - a(n-2) +
 *            a(n-3) (n > 3)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n = 0..3 */
    static const seq_t base[4] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8 };

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* handle trivial small indices */
    if (n < 4)
        return base[n];

    /* first call: allocate cache up to n, compute all terms */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                      /* out‑of‑memory guard */
        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        for (idx_t i = 4; i <= n; ++i) {
            /* a(i) = 2*a(i-1) - a(i-2) + a(i-3) */
            cache[i] = cache[i-1] + cache[i-1] - cache[i-2] + cache[i-3];
        }
        return cache[n];
    }

    /* Cache already exists */
    if (n < cache_size)
        return cache[n];

    /* Extend cache to accommodate up to index n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;                      /* out‑of‑memory guard */
    cache = tmp;

    /* Compute new entries */
    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-1] - cache[i-2] + cache[i-3];
    }

    return cache[n];
}
