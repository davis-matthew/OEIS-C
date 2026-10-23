/*
 * A041211  Denominators of continued fraction convergents to sqrt(116).
 * https://oeis.org/A041211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = u_n for the convergents of sqrt(116) where * u_0 = 1, u_1
 *            = 1 and for n >= 2 * u_n = c_{n-1} * u_{n-1} + u_{n-2} * with
 *            the periodic coefficient sequence c = {1,3,2,1,4,1,2,3,1,20} *
 *            repeated indefinitely. This recurrence produces the *
 *            denominators of the continued‑fraction convergents to...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned int period[10] = {1,3,2,1,4,1,2,3,1,20};

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n < cache_size)        /* already computed */
        return cache[(size_t)n];

    /* Grow cache to accommodate up to index n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp && new_size > 0)   /* allocation failed */
        abort();
    cache = tmp;

    /* Compute new entries.  We rely on the recurrence
     * u_n = c_{n-1} * u_{n-1} + u_{n-2}.  The
     * coefficient c_{n-1} is taken from period[(n-1)%10]. */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            /* For n = 1: coeff = period[0] = 1, u_{-1} = 0. */
            cache[1] = (seq_t)period[0] * cache[0];
        } else {
            unsigned int coeff = period[(i - 1) % 10];
            cache[i] = (seq_t)coeff * cache[i - 1] + cache[i - 2];
        }
    }
    cache_size = new_size;
    return cache[(size_t)n];
}
