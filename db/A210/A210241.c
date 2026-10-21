/*
 * A210241  Partial sums of A073093.
 * https://oeis.org/A210241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A022559(n) + n
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A022559(n) is Σ Ω(k) for k = 1..n, where Ω(k) is the total
   number of prime factors of k counted with multiplicity. */

#include <stdlib.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* sequence is defined for n ≥ 1 */

    /* Static state */
    static idx_t *spf = NULL;           /* smallest prime factor table */
    static idx_t spf_limit = 0;         /* highest index currently in spf */

    static idx_t *bigomega = NULL;      /* Ω(k) array */
    static idx_t bigomega_limit = 0;    /* highest index precomputed */

    static seq_t *prefix = NULL;        /* cumulative sum of Ω(k) */
    static idx_t prefix_limit = 0;      /* highest index precomputed */

    /* Build/extend SPF table up to n */
    if (n > spf_limit) {
        if (spf)
            free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    /* Extend Ω(k) array if needed */
    if (n > bigomega_limit) {
        idx_t new_limit = n;
        idx_t *new_bm = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
        if (!new_bm)
            return 0; /* allocation failure */

        /* Copy old values or initialise */
        if (bigomega) {
            for (idx_t i = 0; i <= bigomega_limit; ++i)
                new_bm[i] = bigomega[i];
            free(bigomega);
        } else {
            new_bm[0] = 0;
            if (new_limit >= 1)
                new_bm[1] = 0;
        }

        /* Compute Ω(k) for the new range */
        for (idx_t k = bigomega_limit + 1; k <= new_limit; ++k) {
            if (k == 1) {
                new_bm[k] = 0;
            } else {
                new_bm[k] = new_bm[k / spf[k]] + 1;
            }
        }
        bigomega = new_bm;
        bigomega_limit = new_limit;
    }

    /* Extend prefix sum array if needed */
    if (n > prefix_limit) {
        idx_t new_limit = n;
        seq_t *new_pref = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
        if (!new_pref)
            return 0; /* allocation failure */

        new_pref[0] = 0;

        if (prefix) {
            for (idx_t i = 0; i <= prefix_limit; ++i)
                new_pref[i] = prefix[i];
            free(prefix);
        }

        for (idx_t k = prefix_limit + 1; k <= new_limit; ++k)
            new_pref[k] = new_pref[k - 1] + (seq_t)bigomega[k];

        prefix = new_pref;
        prefix_limit = new_limit;
    }

    /* a(n) = Σ Ω(k) for k=1..n + n = prefix[n] + n */
    return prefix[n] + (seq_t)n;
}
