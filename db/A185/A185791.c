/*
 * A185791  Number of (n+1) X 3 binary arrays with no 2 X 2 subblock sum equal
 *          to any horizontal or vertical neighbor 2 X 2 subblock sum.
 * https://oeis.org/A185791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + 2*a(n‑2) + 16*a(n‑4) – 16*a(n‑5) for n ≥ 6 with
 *            a(1)=40, a(2)=96, a(3)=224, a(4)=576, a(5)=1408
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* OEIS offset is 1 → a(1) is the first term.  For n=0 return 0. */
    if (n == 0)
        return 0;

    static seq_t *cache = NULL;   /* cache[1] … cache[max_computed] */
    static size_t  cap   = 0;     /* allocated size of cache (in elements) */
    static size_t  computed = 0;  /* highest index that has been filled */

    /* Ensure the cache can hold indices up to n (indices 1…n). */
    size_t needed = (size_t)n + 1;              /* include 0‑slot for simplicity */
    if (needed > cap) {
        size_t newcap = cap ? cap : 8;
        while (newcap < needed)
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialize the first five terms once. */
    if (computed < 5) {
        cache[1] = 40;
        cache[2] = 96;
        cache[3] = 224;
        cache[4] = 576;
        cache[5] = 1408;
        computed = 5;
    }

    /* If the desired term is within the preinitialised range return it. */
    if ((size_t)n <= computed)
        return cache[(size_t)n];

    /* Compute forward from the next uncomputed index up to n using the recurrence. */
    for (size_t k = computed + 1; k <= (size_t)n; ++k) {
        cache[k] = cache[k-1]
                 + (seq_t)2 * cache[k-2]
                 + (seq_t)16 * cache[k-4]
                 - (seq_t)16 * cache[k-5];
    }
    computed = (size_t)n;
    return cache[(size_t)n];
}
