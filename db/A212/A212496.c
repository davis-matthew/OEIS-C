/*
 * A212496  a(n) = Sum_{k=1..n} (-1)^(k-Omega(k)) with Omega(k) the total
 *          number of prime factors of k (counted with multiplicity).
 * https://oeis.org/A212496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k - Omega(k)) (Omega = total number of
 *            prime factors, counted with multiplicity)
 * Range:     i8 n <= 5423, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"   /* Provides void build_spf(idx_t limit, idx_t **spf_out); */

seq_t a(idx_t n) {
    /* Static cache of partial sums and spf table */
    static seq_t *cum = NULL;          /* cum[k] = sum_{i=1..k} (-1)^(i - Omega(i)) */
    static idx_t cum_cap = 0;          /* maximum k for which cum is valid (in terms of elements, cum has size cum_cap+1) */
    static idx_t *spf = NULL;          /* smallest prime factor for each i */
    static idx_t spf_cap = 0;          /* maximum i for which spf is valid (in terms of elements, spf has size spf_cap+1) */

    if (n == 0) return 0;              /* offset starts at 1, but return 0 for safety */

    /* Ensure spf table covers 1..n */
    if (n > spf_cap) {
        /* free old spf array if it exists */
        if (spf) free(spf);
        build_spf(n, &spf);
        spf_cap = n;
    }

    /* Ensure cumulative array covers 1..n */
    if (n > cum_cap) {
        idx_t new_cap = n;
        seq_t *new_cum = (seq_t *)realloc(cum, (new_cap + 1) * sizeof(seq_t));
        if (!new_cum) exit(1);  /* out of memory */
        cum = new_cum;
        /* Initialize new entries */
        for (idx_t i = cum_cap + 1; i <= new_cap; ++i) {
            /* Compute Omega(i) using spf */
            idx_t m = i;
            uint8_t omega = 0;
            while (m > 1) {
                omega += 1;
                m /= spf[m];
            }
            /* Sign: +1 if parity(i) == parity(omega), else -1 */
            int sign = ((i & 1) == (omega & 1)) ? 1 : -1;
            cum[i] = cum[i - 1] + sign;
        }
        cum_cap = new_cap;
    }

    return cum[n];
}
