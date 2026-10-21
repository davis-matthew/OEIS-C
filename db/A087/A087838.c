/*
 * A087838  a[n] =a[a[a[a[n-2]]]]+ a[n - a[n-2]].
 * https://oeis.org/A087838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(a(a(n-2)))) + a(n - a(n-2)), a(1)=1, a(2)=1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Custom integer types are defined by the build system.
   idx_t is an unsigned integer type, seq_t is the integer type of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;          /* cache[1]..cache[max]   */
    static idx_t capacity = 0;           /* allocated slots        */
    static idx_t max_computed = 0;       /* highest n computed so far */

    /* Ensure the cache can hold at least n+1 indices (index 0 unused) */
    if (n + 1 > capacity) {
        /* Grow to double size or minimal needed size */
        idx_t new_cap = capacity ? capacity * 2 : 32;
        if (new_cap < n + 1)
            new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Initialise base values if not yet done */
    if (max_computed < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* If requested term already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute terms sequentially until n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* a[k] depends on a[k-2] which is already computed */
        seq_t prev = cache[k - 2];
        /* term1 = a( a( a( a( k-2 ) ) ) ) */
        seq_t term1 = cache[(idx_t)cache[(idx_t)cache[(idx_t)cache[k - 2]]]];
        /* term2 = a( k - a[k-2] ) */
        seq_t term2 = cache[k - (idx_t)prev];
        cache[k] = term1 + term2;
    }

    max_computed = n;
    return cache[n];
}
