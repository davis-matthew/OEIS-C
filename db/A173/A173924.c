/*
 * A173924  Expansion of 1/(1 - x^5 - x^6 - x^7 - x^8 + x^13).
 * https://oeis.org/A173924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-6) + a(n-7) + a(n-8) - a(n-13); Initial
 *            values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=1,
 *            a(7)=1, a(8)=1, a(9)=0, a(10)=1, a(11)=2, a(12)=3
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 115, u64 n <= 222,
 *            u128 n <= 436, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values   */
    static size_t  cap   = 0;     /* allocated capacity */
    static size_t  size  = 0;     /* number of filled elements */

    /* Ensure the cache is built with the first 13 terms. */
    if (cache == NULL) {
        /* Allocate a small initial buffer. */
        cap  = 64;                 /* arbitrary small start */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        /* Exact first 13 terms. */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)0;
        cache[3]  = (seq_t)0;
        cache[4]  = (seq_t)0;
        cache[5]  = (seq_t)1;
        cache[6]  = (seq_t)1;
        cache[7]  = (seq_t)1;
        cache[8]  = (seq_t)1;
        cache[9]  = (seq_t)0;
        cache[10] = (seq_t)1;
        cache[11] = (seq_t)2;
        cache[12] = (seq_t)3;
        size     = 13;
    }

    /* Expand the cache if the requested index lies beyond current capacity. */
    if (n >= size) {
        /* Grow the buffer until it can hold indices up to n. */
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n (recurrence only applies when i >= 13). */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 13) {
            /* The first 13 terms are already initialized. */
            continue;
        }
        cache[i] = cache[i-5] + cache[i-6] + cache[i-7] + cache[i-8] - cache[i-13];
    }
    if ((size_t)n + 1 > size) size = (size_t)n + 1;

    return cache[n];
}
