/*
 * A041321  Denominators of continued fraction convergents to sqrt(174).
 * https://oeis.org/A041321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2902*a(n-4) - a(n-8) a(0)=1, a(1)=5, a(2)=21, a(3)=110,
 *            a(4)=2881, a(5)=14515, a(6)=60941, a(7)=319220
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* for size_t */
#include "../utils/seq_alloc.h"  /* seq_alloc */
#include "../utils/seq_realloc.h"/* seq_realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  size  = 0; /* number of cached terms */
    static size_t  cap   = 0; /* allocated capacity */

    /* initial eight terms as required by the recurrence */
    const seq_t init[8] = {
        (seq_t)1,    (seq_t)5,    (seq_t)21,
        (seq_t)110,  (seq_t)2881, (seq_t)14515,
        (seq_t)60941,(seq_t)319220
    };

    if (size == 0) {                /* first call: initialise cache */
        size = 8;
        cap  = 8;
        cache = seq_alloc(cap);
        for (size_t i = 0; i < size; ++i)
            cache[i] = init[i];
    }

    /* return already computed term */
    if ((idx_t)size > n)
        return cache[(size_t)n];

    /* grow array if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap;
        while (new_cap < needed)
            new_cap <<= 1; /* double */
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = size; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)2902 * cache[i - 4] - cache[i - 8];
    }
    size = n + 1;

    return cache[(size_t)n];
}
