/*
 * A169064  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the rational generating function G(t) = (1 +
 *            t + … + t²⁴) / (1 - 2t + t² - 2t³ + … + t²⁴). For n ≥ 25 the
 *            sequence satisfies the linear recurrence a(n) = 2a(n-1) - a(n-2)
 *            + 2a(n-3) - a(n-4) + … + 2a(n-23) - a(n-24). a(24) is obtained
 *            from the same pattern with an added constant...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* include the helper that resizes dynamic arrays */
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* values computed so far      */
    static size_t cap   = 0;           /* allocated capacity          */
    static idx_t cached = 0;           /* highest index computed      */

    /* initial values for indices 0..23 */
    static const seq_t init[24] = {
        1, 3, 6, 12, 24, 48, 96, 192, 384, 768,
        1536, 3072, 6144, 12288, 24576, 49152,
        98304, 196608, 393216, 786432, 1572864,
        3145728, 6291456, 12582912
    };

    /* first call – initialise cache with 0..23 and compute a(24) */
    if (cache == NULL) {
        grow_seq(&cache, &cap, 25);  /* space for indices 0..24   */
        for (size_t i = 0; i < 24; ++i)
            cache[i] = init[i];
        cached = 23;

        /* compute a(24) */
        seq_t val = 1;
        for (idx_t k = 1; k <= 24; ++k) {
            seq_t term = cache[24 - k];
            if (k & 1)          /* odd k → +2 * term */
                val += term + term;
            else                /* even k → -1 * term */
                val -= term;
        }
        cache[24] = val;
        cached = 24;
    }

    /* already computed? */
    if (n <= cached)
        return cache[n];

    /* enlarge cache as necessary */
    if ((idx_t)cap <= n)
        grow_seq(&cache, &cap, (size_t)(n + 1));

    /* extend cache up to n using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val = 0;
        for (idx_t k = 1; k <= 24; ++k) {
            seq_t term = cache[i - k];
            if (k & 1)          /* odd k → +2 * term */
                val += term + term;
            else                /* even k → -1 * term */
                val -= term;
        }
        cache[i] = val;
    }
    cached = n;

    return cache[n];
}
