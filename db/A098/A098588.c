/*
 * A098588  a(n) = 2^n for n = 0..4; for n > 4, a(n) = 2*a(n-1) + a(n-5).
 * https://oeis.org/A098588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 16; for n > 4:
 *            a(n) = 2*a(n-1) + a(n-5)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a cache of already computed terms.  Cached
   values are stored in a static array that expands on demand.  For
   n < 5 the values are simply powers of two.  For n ≥ 5 the linear
   recurrence is applied iteratively.  The algorithm is O(n) time and
   uses O(n) memory, which easily satisfies the 2‑second time limit
   for n up to 10 000.  The arithmetic uses only the operators
   allowed for seq_t (shifts and +), so it works for any integer
   type defined by the build.  */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = 0;
    static size_t cap = 0;      /* capacity of the cache */
    static size_t top = 0;      /* index of first not yet computed term */

    /* Initial allocation and first five terms */
    if (cache == 0) {
        /* Allocate space for at least 16 entries */
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        cap   = 16;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)16;
        top = 5;      /* next index that needs to be filled */
    }

    /* Ensure the cache can hold index n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = (cap == 0) ? 16 : cap << 1;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Return directly if already computed */
    if ((size_t)n < top)
        return cache[n];

    /* Compute missing terms up to n */
    for (size_t i = top; i <= n; ++i) {
        if (i < 5) {
            cache[i] = (seq_t)1 << i;            /* 2^i */
        } else {
            cache[i] = (cache[i-1] << 1) + cache[i-5];
        }
    }
    top = (size_t)n + 1;
    return cache[n];
}
