/*
 * A066768  a(n) = Sum_{d|n} binomial(2*d-2,d-1).
 * https://oeis.org/A066768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} binomial(2*d-2, d-1). Offset: 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static size_t cache_size = 0;          /* largest index computed so far */
    static seq_t *cached_a = NULL;        /* a[1..cache_size] */
    static seq_t *cached_g = NULL;        /* g[d] = binomial(2*d-2,d-1) for d <= cache_size */

    if (n == 0)               /* not part of the defined sequence */
        return (seq_t)0;

    /* Extend cache if necessary */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        size_t old_size = cache_size;

        /* Reallocate arrays */
        seq_t *a_new = (seq_t*)realloc(cached_a, (new_size + 1) * sizeof(seq_t));
        seq_t *g_new = (seq_t*)realloc(cached_g, (new_size + 1) * sizeof(seq_t));
        if (!a_new) return (seq_t)0;  /* allocation failure - return 0 */
        if (!g_new) return (seq_t)0;
        cached_a = a_new;
        cached_g = g_new;

        /* Compute g for new indices and recompute all a values */
        for (size_t d = old_size + 1; d <= new_size; ++d) {
            cached_g[d] = binomial((idx_t)(2 * d - 2), (idx_t)(d - 1));
        }

        /* Reset a array */
        for (size_t i = 1; i <= new_size; ++i)
            cached_a[i] = (seq_t)0;

        /* Calculate divisor sums up to new_size */
        for (size_t d = 1; d <= new_size; ++d) {
            seq_t val = cached_g[d];
            for (size_t m = d; m <= new_size; m += d) {
                cached_a[m] += val;
            }
        }

        cache_size = new_size;
    }

    return cached_a[n];
}
