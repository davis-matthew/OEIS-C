/*
 * A041095  Denominators of continued fraction convergents to sqrt(55).
 * https://oeis.org/A041095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   a(n) = 178*a(n-4) - a(n-8) (n ≥ 8) Initial values: a(0)=1,
 *            a(1)=2, a(2)=5, a(3)=12, a(4)=173, a(5)=358, a(6)=889, a(7)=2136
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoised values */
static seq_t *cache = NULL;
static idx_t last_computed = (idx_t)-1;   /* -1 means nothing computed yet */

static const seq_t init_vals[8] = {
    (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)12,
    (seq_t)173, (seq_t)358, (seq_t)889, (seq_t)2136
};

/* Ensure the cache can hold values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate space for at least the first 8 values */
        cache = (seq_t*)malloc(sizeof(seq_t) * (n+1 > 8 ? n+1 : 8));
        /* Initialize the first terms */
        for (idx_t i = 0; i < 8 && i <= n; ++i)
            cache[i] = init_vals[i];
        last_computed = (n >= 7) ? 7 : (idx_t)n;
        return;
    }

    if (n <= last_computed)
        return;                     /* already computed enough */

    /* Grow the cache */
    cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n+1));
    /* Compute new terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i-4] * (seq_t)178 - cache[i-8];
        }
    }
    last_computed = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
