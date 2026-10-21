/*
 * A055448  Base 8 distribution of first digit of mantissa following Benford's
 *          Law, to minimize chi-squared statistic.
 * https://oeis.org/A055448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   greedy choice of the base‑8 first digit that minimises the
 *            chi‑square statistic against Benford’s law. For n ≥ 1 we have
 *            a(1) = 1, a(2) = 2, a(3) = 3, … as given in OEIS A055448. The
 *            function computes the sequence on demand; it stores already
 *            computed terms so that calls may arrive in any order
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   init_probs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/init_probs.h"

/* a(n) – returns the nth term of A055448. */
seq_t a(idx_t n) {
    /* static state */
    static seq_t *cache = NULL;        /* cache[1..computed]  */
    static idx_t cache_size = 0;       /* size of allocated array */
    static uint64_t counts[8];         /* observed counts 1..7  */
    static double probs[8];
    static int probs_ready = 0;

    static idx_t last_computed = 0;    /* highest n already in cache */

    if (!probs_ready) {
        init_probs(probs);
        probs_ready = 1;
        memset(counts, 0, sizeof(counts));
    }

    /* ensure cache is large enough */
    if (n >= cache_size) {
        idx_t new_size = n + 1;             /* slot 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        /* initialise new entries to zero */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        double exp_cnt[8];
        for (int d = 1; d <= 7; ++d)
            exp_cnt[d] = (double)cur * probs[d];   /* expected count for digit d */

        int best_digit = 1;
        double best_chi  = 1e300;                /* very large number */

        /* try each candidate digit */
        for (int cand = 1; cand <= 7; ++cand) {
            double chi = 0.0;
            for (int d = 1; d <= 7; ++d) {
                uint64_t obs = counts[d] + (d == cand ? 1 : 0);
                double diff = (double)obs - exp_cnt[d];
                chi += diff * diff / exp_cnt[d];
            }
            if (chi < best_chi - 1e-12 ||
                (fabs(chi - best_chi) <= 1e-12 && cand < best_digit)) {
                best_chi   = chi;
                best_digit = cand;
            }
        }

        counts[best_digit]++;          /* update frequencies   */
        cache[cur] = (seq_t)best_digit; /* store result        */
        last_computed = cur;
    }

    return cache[n];
}
