/*
 * A097677  E.g.f.: (1/(1-x^3))*exp( 3*sum_{i>=0} x^(3*i+1)/(3*i+1) ) for an
 *          order-3 linear recurrence with varying coefficients.
 * https://oeis.org/A097677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=9, and for n≥3 * a(n) = 3 * a(n-1) +
 *            (n!/(n-3)!) * a(n-3) * where n!/(n-3)! = n * (n-1) * (n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache   = NULL;     /* array of terms */
    static idx_t  cap    = 0;        /* allocated size of cache */
    static idx_t  computed = 0;      /* number of terms already computed */

    /* enlarge if necessary to hold at least n+1 entries */
    if (n + 1 > computed) {
        /* ensure at least 3 slots for the base cases */
        if (cap < 3) cap = 3;
        while (cap < n + 1) cap <<= 1;  /* grow exponentially */

        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) abort();              /* fatal error: out of memory */
        cache = tmp;

        /* initialise base cases if this is the first time */
        if (computed < 3) {
            cache[0] = 1;
            cache[1] = 3;
            cache[2] = 9;
            computed = 3;
        }

        /* compute new terms up to n */
        for (idx_t i = computed; i <= n; ++i) {
            /* for i>=3 the recurrence holds */
            seq_t fact = (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2);
            cache[i]   = 3 * cache[i - 1] + fact * cache[i - 3];
        }
        computed = n + 1;
    }

    return cache[n];
}
