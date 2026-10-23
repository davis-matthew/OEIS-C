/*
 * A169178  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the rational generating function of * OEIS
 *            A169178. The sequence satisfies the linear recurrence * * a(0) =
 *            1 * a(n) = 2 + 19 * (a(n-1)+...+a(n-26)) for 1 ≤ n ≤ 26 * a(27)
 *            = 1 + 19 * (a(26)+...+a(1)) * a(n) = 19 * (a(n-1)+...+a(n-26)) -
 *            190 * a(n-27) for n ≥ 28 * * The first 16...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Basic types seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache   = NULL;   /* pointer to stored values      */
    static size_t  cap    = 0;      /* allocated capacity             */
    static idx_t   last   = (idx_t)-1; /* last index computed           */
    static seq_t   sum26  = 0;      /* sum of the previous up to 26 terms */

    /* helper: ensure cache has space for indices up to n */
    if (cap <= n) {
        size_t new_cap = cap? cap<<1 : 16;          /* double or start at 16 */
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory, return 0 */
        if (cache) {
            memcpy(new_cache, cache, (last+1) * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* initialize the very first term if it has not yet been set */
    if (last == (idx_t)-1) {
        cache[0] = 1;
        sum26    = 0;
        last     = 0;
    }

    /* if the requested index is already known, return it */
    if (n <= last) return cache[n];

    /* compute missing values in increasing order */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* update sliding window sum of up to 26 previous terms */
        sum26 += cache[i-1];
        if (i > 26) sum26 -= cache[i-27];

        if (i <= 27) {
            /* coefficients from the numerator of the g.f. */
            seq_t coeff = (i == 27) ? 1 : 2;
            cache[i] = coeff + (seq_t)19 * sum26;
        } else {
            cache[i] = (seq_t)19 * sum26 - (seq_t)190 * cache[i-27];
        }
    }

    last = n;
    return cache[n];
}
