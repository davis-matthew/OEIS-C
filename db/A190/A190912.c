/*
 * A190912  Partial sums of pentanacci numbers (A000322).
 * https://oeis.org/A190912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-6) Initial values: a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=4, a(5)=5, a(6)=10
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 68, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 */
    if (n == 0) return (seq_t)0;

    /* 1‑based static cache of computed values */
    static seq_t *cache = NULL;   /* cache[1] == a(1) */
    static size_t max_computed = 0; /* largest index stored so far */
    static size_t cap = 0;         /* allocated size – 1 (max index stored) */

    size_t tn = (size_t)n;        /* cast for array indices */

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        /* Reserve space for at least the first 6 terms to simplify init */
        cap = (tn < 6 ? 6 : tn);
        cache = (seq_t*)malloc(sizeof(seq_t) * (cap + 1));
        if (!cache) return (seq_t)0; /* allocation failed */
    } else if (tn > cap) {
        size_t new_cap = tn;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the first six values on the first request */
    if (max_computed == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)10;
        max_computed = 6;
    }

    /* If requested value is already cached, return it */
    if (tn <= max_computed)
        return cache[tn];

    /* Compute the missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)(max_computed + 1); i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] - cache[i - 6];
    }
    max_computed = tn;

    return cache[n];
}
