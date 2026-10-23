/*
 * A024537  a(n) = floor( a(n-1)/(sqrt(2) - 1) ), with a(0) = 1.
 * https://oeis.org/A024537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 1, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values.  Cache[0] = 1, Cache[1] = 2. */
    static seq_t *cache = NULL;
    static idx_t size = 0;      /* Number of cached entries, 0..size-1 */

    /* Initialise cache on first call. */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 2);
        if (!cache) return (seq_t)0;   /* Out‑of‑memory guard, unlikely here. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        size   = 2;
    }

    /* Grow the cache up to n (inclusive) if required. */
    while (size <= n) {
        seq_t val = ((seq_t)2 * cache[size-1]) + cache[size-2] - (seq_t)1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (size+1));
        if (!tmp) return (seq_t)0;   /* Guard for memory failure. */
        cache = tmp;
        cache[size] = val;
        ++size;
    }

    return cache[n];
}
