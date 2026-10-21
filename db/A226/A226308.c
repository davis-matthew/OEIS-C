/*
 * A226308  a(n) = a(n-1) + a(n-2) + 2*a(n-3) with a(0)=2, a(1)=1, a(2)=5.
 * https://oeis.org/A226308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3), with a(0)=2, a(1)=1, a(2)=5
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to keep already computed values */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;   /* largest index already filled in cache */
    static size_t cap   = 0;  /* current allocated capacity of cache */

    /* initialization for first few terms */
    if (cap == 0) {
        /* allocate space for indices 0..2 */
        cap   = 3;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        max_n   = 2;
    }

    /* if requested n is already in cache, return it */
    if (n <= max_n)
        return cache[n];

    /* extend cache to accommodate up to n */
    if ((idx_t)(cap - 1) < n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort (should not happen in test harness) */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) + 2*a(i-3) */
        cache[i] = cache[i - 1] + cache[i - 2] + (cache[i - 3] * (seq_t)2);
    }
    max_n = n;
    return cache[n];
}
