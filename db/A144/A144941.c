/*
 * A144941  Numbers k such that 6*k-1 = A144796(k).
 * https://oeis.org/A144941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144941: numbers k such that 6*k-1 = A144796(k).
   The sequence satisfies the recurrences
     a(1) = 1,  a(2) = 36,  a(3) = 753,  a(4) = 41348
     a(n) = 1154*a(n-2) - a(n-4) - 192   for every n >= 5
   (applied separately to odd and even indices).  */

/* Include required headers for both C and C++ */
#include <stdlib.h>

/* External linkage, fetch from the build system. */
seq_t
a(idx_t n)
{
    /* Static cache of computed values (1‑indexed). */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t  cap   = 0;      /* current allocated size   */
    static idx_t  size  = 0;      /* highest index computed   */

    /* Grow the cache if needed. */
    if (n >= cap) {
        idx_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= n)
            new_cap *= 2;

        /* Allocate new cache. */
#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
#endif
        if (!new_cache) return 0; /* Out of memory: return zero. */

        /* Copy existing values. */
        for (idx_t i = 1; i <= size; ++i)
            new_cache[i] = cache[i];

        /* Free old cache. */
#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 36;
        } else if (i == 3) {
            cache[i] = 753;
        } else if (i == 4) {
            cache[i] = 41348;
        } else {
            /* Recurrence: a(n) = 1154*a(n-2) - a(n-4) - 192 */
            cache[i] = cache[i - 2] * 1154 - cache[i - 4] - 192;
        }
    }
    size = n;

    return cache[n];
}
