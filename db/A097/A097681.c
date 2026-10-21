/*
 * A097681  E.g.f.: (1/(1-x^6))*exp( 6*sum_{i>=0} x^(6*i+1)/(6*i+1) ) for an
 *          order-6 linear recurrence with varying coefficients.
 * https://oeis.org/A097681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 6^n for n < 6 a(n) = 6*a(n-1) +
 *            (n!/(n-6)!)*a(n-6) for n ≥ 6 The implementation memoises results
 *            in a static array, allocating as needed. It uses 128‑bit
 *            arithmetic for the intermediate multiplication to avoid overflow
 *            on most platforms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoised values */
static seq_t *cache = NULL;          /* array of computed values */
static size_t cached = 0;            /* number of values stored (cache[0..cached-1]) */

/* Compute the nth term */
seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if ((size_t)n >= cached) {
        size_t newsize = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure, should not happen */

        /* Fill new entries */
        for (size_t i = cached; i <= (size_t)n; ++i) {
            if (i < 6) {
                /* a(i) = 6^i : build multiplicatively */
                if (i == 0)
                    cache[i] = (seq_t)1;
                else
                    cache[i] = (seq_t)( (unsigned __int128)6 * (unsigned __int128)cache[i-1] );
            } else {
                /* Compute factorial part (i! / (i-6)!) = i*(i-1)*...*(i-5) */
                unsigned __int128 fpart = 1;
                for (uint64_t k = 0; k < 6; ++k)
                    fpart *= (unsigned __int128)(i - k);

                /* Compute recurrence using 128‑bit intermediate */
                unsigned __int128 term = (unsigned __int128)6 * (unsigned __int128)cache[i-1]
                                       + fpart * (unsigned __int128)cache[i-6];

                cache[i] = (seq_t)term;
            }
        }
        cached = newsize;
    }

    return cache[(size_t)n];
}
