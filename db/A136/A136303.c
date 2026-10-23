/*
 * A136303  Expansion of g.f. (1 +x^2)/((1-x)^2*(1 -3*x +2*x^2 -x^3)).
 * https://oeis.org/A136303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+5) = 5*a(n+4) -9*a(n+3) +8*a(n+2) -4*a(n+1) +a(n), a(0)=1,
 *            a(1)=5, a(2)=17, a(3)=48, a(4)=123
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed terms.  The array grows as needed. */
    static seq_t *cache = NULL;
    static idx_t max_computed = (idx_t)-1;   /* sentinel for uninitialised cache */

    /* Initialisation on first call */
    if (cache == NULL) {
        /* At least hold the first five terms */
        idx_t init_size = 5;
        if (n + 1 > init_size)
            init_size = n + 1;
        cache = (seq_t *)malloc((size_t)init_size * sizeof(seq_t));
        if (cache == NULL)
            return 0;                     /* out‑of‑memory: safety fall‑back */

        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 17;
        cache[3] = 48;
        cache[4] = 123;
        max_computed = 4;
    }

    /* Grow the cache if the requested index is beyond what we have */
    if (n > max_computed) {
        /* Ensure capacity: double until big enough */
        idx_t need = n + 1;
        idx_t cap = (max_computed + 1) * 2;
        while (cap < need)
            cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                     /* out‑of‑memory: safety fall‑back */
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = ((seq_t)5 * cache[i - 1]
                         - (seq_t)9 * cache[i - 2]
                         + (seq_t)8 * cache[i - 3]
                         - (seq_t)4 * cache[i - 4]
                         + cache[i - 5]);
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
