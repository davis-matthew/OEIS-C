/*
 * A117585  a(n) = 2*a(n-1) + a(n-2) + n.
 * https://oeis.org/A117585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117585
   a(n) = 2*a(n-1) + a(n-2) + n,
   a(0) = 1, a(1) = 3
   (i.e. a recurrence that starts being valid for n ≥ 2) */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size, in terms of seq_t elements   */
    static size_t len   = 0;   /* number of already computed elements (0 … len-1) */

    /* allocate, on first invocation, space for at least indices 0 and 1 */
    if (!cache) {
        cap = (n > 1) ? (size_t)n + 1 : 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* simple error fall‑back (should never happen) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        len     = 2;
    }

    /* make sure the allocated buffer is large enough for index n */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* simple error fall‑back */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing values up to n, using the recurrence */
    if (n >= len) {
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 2] + (seq_t)i;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
