/*
 * A223419  3-level binary fanout graph coloring a rectangular array: number
 *          of n X 4 0..6 arrays where 0..6 label nodes of a graph with edges
 *          0,1 1,3 1,4 0,2 2,5 2,6 and every array movement to a horizontal
 *          or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A223419: 3‑level binary fanout graph coloring
 * Recurrence (for n ≥ 7):
 *   a(n) = 14*a(n-1) - 36*a(n-2) - 40*a(n-3) + 88*a(n-4) + 32*a(n-5) - 32*a(n-6)
 * Initial values:
 *   a(1)=48, a(2)=464, a(3)=4720, a(4)=47872, a(5)=486016, a(6)=4934272
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly.  Indices 1..6 are defined. */
    static const seq_t base[7] = {0,
        (seq_t)48, (seq_t)464, (seq_t)4720,
        (seq_t)47872, (seq_t)486016, (seq_t)4934272};

    if (n <= 6)
        return base[n];

    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity (indices 0..cap-1) */
    static idx_t   last  = 0;    /* largest n stored in cache */

    /* Initialize cache on first use. */
    if (!cache) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failed – caller should handle */
        /* cache[0] is unused */
        cache[1] = (seq_t)48;
        cache[2] = (seq_t)464;
        cache[3] = (seq_t)4720;
        cache[4] = (seq_t)47872;
        cache[5] = (seq_t)486016;
        cache[6] = (seq_t)4934272;
        last   = 6;
    }

    /* Grow the cache if needed. */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;                  /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        cache[i] = ((seq_t)14) * a1
                 - ((seq_t)36) * a2
                 - ((seq_t)40) * a3
                 + ((seq_t)88) * a4
                 + ((seq_t)32) * a5
                 - ((seq_t)32) * a6;
    }
    last = n;
    return cache[n];
}
