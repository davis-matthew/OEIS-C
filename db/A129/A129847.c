/*
 * A129847  a(n) = number of set partitions of {1, 2, ..., n} whose blocks
 *          consist only of elements that differ by two or less (that is, have
 *          only the forms {i}, {i,i+1}, {i,i+2}, or {i,i+1,i+2}).
 * https://oeis.org/A129847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+2*a(n-3)+a(n-4), with a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* memoization cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   last  = 3;      /* last computed index */

    /* initialize cache on first call */
    if (!cache) {
        cap   = 4;               /* indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);     /* out‑of‑memory guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
    }

    /* already computed? */
    if (n <= last)
        return cache[n];

    /* extend buffer if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(1);         /* out‑of‑memory guard */
        cache = tmp;
        cap   = needed;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term =
            cache[i-1]                      /* a(i‑1) */
          + cache[i-2]                      /* a(i‑2) */
          + ((seq_t)2 * cache[i-3])         /* 2*a(i‑3) */
          + cache[i-4];                    /* a(i‑4) */
        cache[i] = term;
    }

    last = n;
    return cache[n];
}
