/*
 * A041193  Denominators of continued fraction convergents to sqrt(107).
 * https://oeis.org/A041193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041193: denominators of continued‑fraction convergents of sqrt(107)
   Recurrence: a(n) = 1924*a(n-6) - a(n-12)
   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=29, a(4)=32, a(5)=93,
                   a(6)=1892, a(7)=3877, a(8)=5769, a(9)=55798,
                   a(10)=61567, a(11)=178932
   The function is memoised and works for any seq_t size (8/16/32/64/128 bits or
   arbitrary precision).                                                  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated cache size */
    static size_t computed = 0;     /* number of terms already computed */

    /* initialise the cache with the first 12 terms on the first call */
    if (!cache) {
        cap = 16;                      /* initial capacity (will grow if needed) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */

        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)2;
        cache[2]  = (seq_t)3;
        cache[3]  = (seq_t)29;
        cache[4]  = (seq_t)32;
        cache[5]  = (seq_t)93;
        cache[6]  = (seq_t)1892;
        cache[7]  = (seq_t)3877;
        cache[8]  = (seq_t)5769;
        cache[9]  = (seq_t)55798;
        cache[10] = (seq_t)61567;
        cache[11] = (seq_t)178932;

        computed = 12;
    }

    /* if the requested term is already computed, return it */
    if (n < computed)
        return cache[n];

    /* grow the cache if necessary to accommodate index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;                /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* reallocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms in order */
    for (idx_t i = computed; i <= n; ++i) {
        /* recurrence: a(i) = 1924*a(i-6) - a(i-12) */
        cache[i] = ((seq_t)1924 * cache[i - 6]) - cache[i - 12];
    }
    computed = (size_t)n + 1;

    return cache[n];
}
