/*
 * A042268  Numerators of continued fraction convergents to sqrt(660).
 * https://oeis.org/A042268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042268: Numerators of continued‑fraction convergents to sqrt(660)  
   Formula: a(n) = 2158*a(n-6) – a(n-12)     
   Initial values: a(0)=25, a(1)=26, a(2)=77, a(3)=334, a(4)=745, a(5)=1079,
                    a(6)=54695, a(7)=55774, a(8)=166243, a(9)=720746,
                    a(10)=1607735, a(11)=2328481   */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms for indices 0…11 */
    static const seq_t init[12] = {
        25U, 26U, 77U, 334U, 745U, 1079U,
        54695U, 55774U, 166243U, 720746U,
        1607735U, 2328481U
    };

    /* If n is one of the initial terms, return immediately. */
    if (n < 12)
        return init[n];

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cached_upto = 0;   /* largest index already computed in cache */
    static size_t cap = 0;           /* allocated size of cache (elements) */

    /* Allocate cache on first use. */
    if (!cache) {
        /* Ensure we have at least enough room for n + 1 elements. */
        cap = (size_t)n + 1;
        if (cap < 128)
            cap = 128;                 /* small default size, grow if needed later */

        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)  /* allocation failure */
            return 0;

        /* Initialise the first 12 elements. */
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];

        cached_upto = 11;  /* last index known so far */
    }

    /* If the requested index is within the already computed range, return it. */
    if (n <= cached_upto)
        return cache[n];

    /* Grow the cache if the requested index exceeds the current capacity. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        if (new_cap < 128)
            new_cap = 128;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;       /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* reallocation failure */
            return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute successive terms up to n using the recurrence. */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        cache[i] = (seq_t)2158 * cache[i - 6] - cache[i - 12];
    }
    cached_upto = n;

    return cache[n];
}
