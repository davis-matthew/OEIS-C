/*
 * A065094  a(1) = 1, a(n+1) is the sum of a(n) and floor( arithmetic mean of
 *          a(1) ... a(n) ).
 * https://oeis.org/A065094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 163, u64 n <= 576,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A065094:  a(1) = 1, a(n+1) = a(n) + floor((a(1)+...+a(n))/n)  */
/*  The implementation uses a static cache and keeps a running sum.  */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <type_traits>
/* Use a wider type for the sum when seq_t is a builtin integral type.  */
using sum_t = std::conditional_t< (sizeof(seq_t) <= 8), unsigned __int128, seq_t >;
#else
/* In C we can rely on the compiler providing unsigned __int128.  */
typedef unsigned __int128 sum_t;
#endif

seq_t a(idx_t n)
{
    /* static cache and state */
    static seq_t *cache = NULL;      /* a[1]..a[computed] */
    static size_t cap = 0;           /* allocated capacity (indices 0..cap-1) */
    static idx_t computed = 0;       /* highest computed index (>=1) */
    static sum_t sum = 0;            /* running sum of a(1)..a(computed) */

    /* initialise on first call */
    if (computed == 0) {
        cap = 2;                               /* indices 0 and 1 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[1] = (seq_t)1;
        sum = (sum_t)1;
        computed = 1;
    }

    /* if requested term already computed, return it */
    if (n <= computed) {
        return cache[n];
    }

    /* grow cache as needed */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap ? cap * 2 : 2;
        while (newcap <= (size_t)n + 1) {
            newcap *= 2;
        }
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* increment = floor( sum / (i-1) ) */
        seq_t inc = (seq_t)(sum / (sum_t)(i - 1));

        cache[i] = cache[i - 1] + inc;
        sum += (sum_t)cache[i];
    }

    computed = n;
    return cache[n];
}
