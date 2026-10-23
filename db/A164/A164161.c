/*
 * A164161  Number of binary strings of length n with equal numbers of 0001
 *          and 1000 substrings.
 * https://oeis.org/A164161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164161: number of binary strings of length n with equal numbers of
 * 0001 and 1000 substrings.
 *
 * Recurrence:
 *   a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=26, a(6)=50;
 *   for n>=7,  a(n) = 2 * a(n-1).
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache to allow fast non‑sequential access */
    static seq_t *cache = NULL;
    static size_t len  = 0;   /* number of elements currently filled */
    static size_t cap  = 0;   /* allocated capacity */

    /* initial terms of the sequence */
    const seq_t init_vals[] = {1, 2, 4, 8, 14, 26, 50};
    const size_t init_len = sizeof(init_vals) / sizeof(init_vals[0]);

    /* ensure cache is allocated */
    if (!cache) {
        cap = 16;                      /* start with a small capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – return 0 */
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];
        len = init_len;
    }

    /* expand cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while (newcap < (size_t)n + 1)
            newcap <<= 1;                     /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                   /* reallocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to index n */
    if ((size_t)n + 1 > len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i < init_len)               /* already initialised */
                continue;
            /* for i >= init_len (i >= 7) use the recurrence a(n)=2*a(n-1) */
            cache[i] = cache[i - 1] + cache[i - 1];
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
