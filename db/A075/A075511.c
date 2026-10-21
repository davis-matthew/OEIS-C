/*
 * A075511  Sixth column of triangle A075497.
 * https://oeis.org/A075511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n) * S2(n+6, 6) (S2 = Stirling numbers of the second
 *            kind)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1, a(1) = 42, a(2) = 1064, …  (see OEIS A075511) */

#include <stddef.h>
#include <stdlib.h>

#include "../utils/pow2.h"          /* extern seq_t pow2(idx_t exp); */

static idx_t  max_m_precomp = 0;     /* highest m for which Stirling row S[. ] is known */
static seq_t  S[7];                  /* S[k] = S2(m, k) for current m */

seq_t a(idx_t n)
{
    /* we need S2(n+6, 6); ensure row for m = n+6 is computed */
    idx_t target_m = n + 6;

    /* initialise for m = 0 */
    if (max_m_precomp == 0) {
        for (idx_t k = 0; k < 7; ++k)
            S[k] = (k == 0) ? ((seq_t)1) : ((seq_t)0);
    }

    /* incrementally compute rows until target_m */
    while (max_m_precomp < target_m) {
        seq_t newS[7] = {0};

        /* apply recurrence: S(n+1, k) = k*S(n, k) + S(n, k-1) */
        for (idx_t k = 1; k <= 6; ++k) {
            /* term k * S(m, k) */
            seq_t term1 = (seq_t)k * S[k];
            /* term S(m, k-1) */
            seq_t term2 = S[k - 1];
            newS[k] = term1 + term2;
        }
        /* copy back */
        for (idx_t k = 0; k < 7; ++k)
            S[k] = newS[k];

        ++max_m_precomp;
    }

    /* Now S[6] holds S2(n+6, 6) */
    seq_t two_pow_n = pow2(n);
    return two_pow_n * S[6];
}
