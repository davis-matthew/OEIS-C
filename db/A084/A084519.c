/*
 * A084519  Number of indecomposable ground-state 3-ball juggling sequences of
 *          period n.
 * https://oeis.org/A084519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=3*a(n-1)+2*a(n-2)+2*a(n-3), with a(1)=1, a(2)=1, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for indices < 1 */

    static seq_t *cache = NULL;   /* memoisation array */
    static idx_t last = 0;        /* highest index already computed */
    static size_t alloc = 0;      /* allocated size of cache (in elements) */

    /* First call: create a minimal cache to hold the initial values */
    if (!cache) {
        alloc = 4;                /* store indices 1..3 (index 0 unused) */
        cache = (seq_t*)malloc(sizeof(seq_t) * alloc);
        if (!cache) return 0;     /* allocation failure: safe‑return 0 */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        last = 3;
    }

    /* If the requested term has already been computed, return it */
    if (n <= last) return cache[n];

    /* Ensure the array is large enough for indices up to n */
    size_t need = (size_t)n + 1;  /* allocate up to index n */
    if (need > alloc) {
        size_t new_alloc = alloc;
        while (new_alloc < need) new_alloc *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_alloc);
        if (!tmp) return 0;      /* reallocation failure */
        cache = tmp;
        alloc = new_alloc;
    }

    /* Compute any missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t v = 3 * cache[i-1] + 2 * cache[i-2] + 2 * cache[i-3];
        cache[i] = v;
    }
    last = n;

    return cache[n];
}
