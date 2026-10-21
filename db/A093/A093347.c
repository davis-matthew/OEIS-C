/*
 * A093347  A 3-fractal "castle" starting with 0.
 * https://oeis.org/A093347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = w(n) - a(n - w(n)), where w(n) = 3^⌊log₃(n-1)⌋ for n ≥ 2,
 *            and a(1) = 0
 * Range:     u8 n <= 729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* 1‑based array, cache[1] = 0          */
    static size_t  cap     = 0;     /* allocated size (elements)            */
    static size_t  computed = 0;    /* highest index that has been computed */

    if (n == 0) return 0;           /* safety – not used by OEIS            */

    /* initialise cache the first time */
    if (cache == NULL) {
        cap = 16;                   /* arbitrary small start                */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)0;        /* a(1)                                 */
        computed = 1;
    }

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap * 2;
        while ((size_t)n >= newcap) newcap *= 2;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!cache) abort();
        cap = newcap;
    }

    /* compute any new terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        /* compute w(i) = 3^floor(log₃(i-1)) */
        uint64_t pow3 = 1;
        uint64_t limit = (uint64_t)(i - 1);
        while (pow3 * 3ULL <= limit) pow3 *= 3ULL;
        seq_t w = (seq_t)pow3;

        /* recurrence a(i) = w - a(i - w) */
        cache[i] = w - cache[i - (size_t)w];
    }
    computed = (size_t)n;
    return cache[n];
}
