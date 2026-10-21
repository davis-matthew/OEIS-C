/*
 * A238834  For n > 1 the sum of t := floor(n/2) + 1 consecutive previous
 *          terms, the leading t terms when n is even, the
 *          immediately-preceding t terms when n is odd; a(0) = 0, a(1) = 1.
 * https://oeis.org/A238834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(3) = 2 For even n ≥ 2: a(n) = a(n-2) +
 *            a(n/2) For odd n ≥ 5: a(n) = 2*a(n-2) - a((n-3)/2) + a(n-1) (The
 *            recurrence creates all terms; the first few terms are 0, 1, 1,
 *            2, 2, 5, 4, 13, 6, 30, 11,…)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external integer types are supplied by the build system. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* pointer to array of results   */
    static size_t cache_cap = 0;         /* capacity of cache (in elements) */
    static idx_t last_computed = 0;      /* highest index whose value is valid */

    /* initial allocation and seed value for a(0) */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* unlikely, but keep compiler happy */
        cache[0] = (seq_t)0;
        last_computed = 0;
    }

    /* grow cache if needed */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* unlikely, but keep compiler happy */
        cache_cap = new_cap;
    }

    /* compute new terms incrementally */
    for (idx_t i = last_computed + 1; i <= n; ++i) {

        if (i == 0) {                     /* already set */
            continue;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = cache[0] + cache[1];
        } else if (i == 3) {
            cache[3] = (seq_t)2;
        } else if ((i & 1) == 0) {        /* even n */
            cache[i] = cache[i-2] + cache[i/2];
        } else {                          /* odd n ≥ 5 */
            seq_t t1 = cache[i-2];        /* 2*a(i-2)  */
            t1 <<= 1;
            seq_t t2 = cache[(i-3)/2];    /* a((i-3)/2) */
            seq_t t3 = cache[i-1];        /* a(i-1)    */
            cache[i] = (t1 + t3) - t2;
        }
    }
    last_computed = n;
    return cache[n];
}
