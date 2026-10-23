/*
 * A041707  Denominators of continued fraction convergents to sqrt(373).
 * https://oeis.org/A041707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 10236*a(n-5) + a(n-10) for n > 9 with initial values:
 *            a(0)=1, a(1)=3, a(2)=16, a(3)=83, a(4)=265, a(5)=10153,
 *            a(6)=30724, a(7)=163773, a(8)=849589, a(9)=2712540
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of allocated slots */
    static idx_t computed_up_to = 0; /* number of terms already filled */

    /* Initial terms for n = 0..9 */
    static const seq_t init_terms[10] = {
        (seq_t)1,  (seq_t)3,  (seq_t)16, (seq_t)83, (seq_t)265,
        (seq_t)10153, (seq_t)30724, (seq_t)163773, (seq_t)849589, (seq_t)2712540
    };

    /* Allocate initial capacity if needed */
    if (cache_cap == 0) {
        cache_cap = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Grow cache if necessary to reach n+1 elements */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to index n */
    while (computed_up_to <= n) {
        if (computed_up_to <= 9) {
            cache[computed_up_to] = init_terms[computed_up_to];
        } else {
            /* Recurrence: a(n) = 10236*a(n-5) + a(n-10) */
            cache[computed_up_to] =
                (seq_t)10236 * cache[computed_up_to - 5] +
                cache[computed_up_to - 10];
        }
        ++computed_up_to;
    }

    return cache[n];
}
