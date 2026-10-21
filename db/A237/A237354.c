/*
 * A237354  a(n) is the maximum of omega(g)+omega(h) for all decompositions
 *          n=g+h with g>=h>=1.
 * https://oeis.org/A237354
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{g+h=n, g>=h>=1} distinctOmega(g)+distinctOmega(h)
 *            where distinctOmega(m) = number of distinct prime factors of m
 *            (counting each prime only once). This implementation uses a
 *            static cache of smallest‑prime factors and precomputes
 *            distinctOmega up to the largest n requested
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* static cache for smallest prime factors and distinct omega values */
    static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i */
    static size_t spf_cap = 0;          /* largest n for which cache is built */
    static unsigned char *omega = NULL;/* omega[i] = distinctOmega(i) */

    if (n < 2) return 0;   /* undefined for n<2, return 0 */

    /* enlarge cache if needed */
    if ((idx_t)n > spf_cap) {
        size_t new_cap = (size_t)n;

        /* free previous caches */
        if (spf) free(spf);
        if (omega) free(omega);

        /* build smallest prime factor array up to new_cap */
        build_spf((idx_t)new_cap, &spf);

        /* allocate omega array */
        omega = (unsigned char *)malloc((new_cap + 1) * sizeof(unsigned char));
        if (!omega) return 0;   /* allocation failure */

        /* initialise */
        omega[0] = 0;
        if (new_cap >= 1) omega[1] = 0;

        /* compute distinctOmega using the SPF recurrence:
           omega[i] = omega[i/p]        if p divides (i/p) (repeated factor)
                      omega[i/p] + 1   otherwise                 */
        for (size_t i = 2; i <= new_cap; ++i) {
            idx_t p = spf[i];
            idx_t j = i / p;
            if (spf[j] == p)
                omega[i] = omega[j];
            else
                omega[i] = omega[j] + 1;
        }

        spf_cap = new_cap;
    }

    /* search for maximum omega(g)+omega(h) over all decompositions n=g+h,
       with g >= h >= 1; loop over h from 1 to n/2 (inclusive). */
    seq_t best = 0;
    for (idx_t h = 1; h <= n / 2; ++h) {
        idx_t g = n - h;
        seq_t val = (seq_t)omega[(size_t)g] + (seq_t)omega[(size_t)h];
        if (val > best) best = val;
    }

    return best;
}
