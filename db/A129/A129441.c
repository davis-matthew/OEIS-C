/*
 * A129441  Expansion of g.f. (1-x^2-x^3)/((1+x+x^2)*(1-2*x-x^2-x^3+x^4)).
 * https://oeis.org/A129441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 4*a(n-3) + a(n-4) - a(n-6) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=7, a(4)=16, a(5)=39
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n < 6 */
    if (n < 6) {
        static const seq_t base[6] = {
            (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)16, (seq_t)39
        };
        return base[n];
    }

    static seq_t *cache = NULL;
    static idx_t cap = 0;      /* capacity of cache (number of items) */
    static idx_t computed = 0; /* highest index computed + 1 */

    /* ensure capacity for indices up to n */
    if (cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – for safety return zero */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* initialise base terms if not already done */
    if (computed < 6) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)39;
        computed = 6;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val =
            cache[i-1] +
            (seq_t)2 * cache[i-2] +
            (seq_t)4 * cache[i-3] +
            cache[i-4] -
            cache[i-6];
        cache[i] = val;
    }
    computed = n + 1;

    return cache[n];
}
