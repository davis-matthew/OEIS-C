/*
 * A123348  Kekulé numbers for certain benzenoids (see the Cyvin-Gutman book
 *          for details).
 * https://oeis.org/A123348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=3, a(n)=4*a(n-1)+2*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n)=3*A090017(n-1) where A090017 has
   recurrence b(0)=1, b(1)=4, b(k)=4*b(k-1)+2*b(k-2). */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated array size  */
    static size_t last  = 0;   /* highest index computed so far */

    /* Initialize cache for n=0 and n=1. */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed: return 0 */
        cache[0] = (seq_t)0;                 /* a(0) */
        cache[1] = (seq_t)3;                 /* a(1) */
        last = 1;
    }

    if (n == 0) return cache[0];
    if ((size_t)n <= last) return cache[n];

    /* Ensure capacity for requested index. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n * 2 + 1;   /* a bit larger than needed */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* realloc failed: return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] * (seq_t)4 + cache[i-2] * (seq_t)2;
    }
    last = (size_t)n;

    return cache[n];
}
