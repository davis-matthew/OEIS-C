/*
 * A122011  G.f. x^2*(1+x)/(1-12*x+15*x^2-2*x^3).
 * https://oeis.org/A122011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=12*a(n-1)-15*a(n-2)+2*a(n-3), a(1)=0, a(2)=1, a(3)=13
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] is unused, cache[1] = a(1) */
static idx_t  max_cached = 0;   /* largest index already computed */

/* Compute a(n) for any n >= 1, using memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, unused by caller */

    /* Ensure cache large enough for indices 0..n. */
    size_t needed = (size_t)n + 1;            /* indices 0..n inclusive */
    if (!cache || needed > (size_t)(max_cached + 1)) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        /* No need to initialise new elements – they will be set below. */
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)0;
        } else if (i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)13;
        } else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            cache[i] = (seq_t)(a1 * (seq_t)12 - a2 * (seq_t)15 + a3 * (seq_t)2);
        }
    }

    max_cached = n;
    return cache[n];
}
