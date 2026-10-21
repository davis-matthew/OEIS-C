/*
 * A052209  b(n)*b(2*n), b(n) = A001353(n+1).
 * https://oeis.org/A052209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052209
   Recurrence: a(0)=4, a(1)=224, a(2)=11700, a(3)=608384
   a(n) = 56*a(n‑1) - 210*a(n‑2) + 56*a(n‑3) - a(n‑4)   for n≥4
   (derived from the generating function (-4x²+4)/[(1-4x+x²)(1-52x+x²)]). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;          /* allocated capacity */
    static idx_t last  = (idx_t)-1;  /* index of last computed term */

    /* Ensure cache is initialized with the first four terms. */
    if (last == (idx_t)-1) {
        /* Allocate capacity for four terms. */
        cap   = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)224;
        cache[2] = (seq_t)11700;
        cache[3] = (seq_t)608384;
        last = 3;
    }

    /* Fast path for already computed indices. */
    if (n <= last)
        return cache[n];

    /* Ensure sufficient capacity for the requested index. */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while (n + 1 > new_cap)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; abort program (as per contest constraints). */
            abort();
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    const seq_t C56  = (seq_t)56;
    const seq_t C210 = (seq_t)210;

    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)(cache[i-1] * C56
                          - cache[i-2] * C210
                          + cache[i-3] * C56
                          - cache[i-4]);
    }

    last = n;
    return cache[n];
}
