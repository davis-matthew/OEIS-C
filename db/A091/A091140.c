/*
 * A091140  a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3) with initial terms 1, 3, 9.
 * https://oeis.org/A091140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3, a(3)=9
   a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3)  (n ≥ 4)       */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* 1‑based indexing, cache[0] unused */
    static idx_t capacity = 0;    /* allocated array size */
    static idx_t last = 0;        /* largest n already computed */

    /* initialise cache on first call */
    if (!cache) {
        capacity = 4;                 /* enough for indices 0..3 */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;          /* dummy value, not used */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)9;
        last = 3;
    }

    if (n <= last)                     /* already known */
        return cache[n];

    /* enlarge cache if needed */
    if (n >= capacity) {
        size_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        capacity = (idx_t)new_cap;
    }

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        t1 *= (seq_t)2;                     /* 2*a(i-1) */

        seq_t t2 = cache[i - 2];
        t2 *= (seq_t)4;                     /* 4*a(i-2) */

        seq_t t3 = cache[i - 3];
        t3 *= (seq_t)2;                     /* 2*a(i-3) */

        cache[i] = t1 + t2 - t3;            /* a(i) */
    }

    last = n;
    return cache[n];
}
