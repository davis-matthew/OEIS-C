/*
 * A046198  Indices of heptagonal numbers (A000566) which are also pentagonal.
 * https://oeis.org/A046198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*a(n-1) - 63*a(n-2) + a(n-3), a(1)=1, a(2)=42,
 *            a(3)=2585
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cached = 0; /* number of values stored (cache[0] is a(1)) */
    static size_t cap = 0;    /* capacity of the cache */

    if (n == 0) /* undefined for n=0, return 0 as a safeguard */
        return (seq_t)0;

    /* initialise cache with first three terms */
    if (!cache) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;     /* a(1) */
        cache[1] = (seq_t)42;    /* a(2) */
        cache[2] = (seq_t)2585;  /* a(3) */
        cached = 3;
    }

    /* grow cache if necessary */
    if (cached < (size_t)n) {
        size_t required = (size_t)n;
        if (required > cap) {
            size_t new_cap = required * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cap = new_cap;
        }
        /* compute missing terms using the recurrence */
        for (size_t i = cached; i < required; ++i) {
            /* i is 0‑based index for term a(i+1) */
            cache[i] = 63 * cache[i-1] - 63 * cache[i-2] + cache[i-3];
        }
        cached = required;
    }

    return cache[n-1];
}
