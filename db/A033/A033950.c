/*
 * A033950  Refactorable numbers: number of divisors of k divides k. Also
 *          known as tau numbers.
 * https://oeis.org/A033950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033950: Refactorable numbers; nth term is the n-th integer k such that
   tau(k) (number of divisors of k) divides k. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0. */
    if (n == 0) return (seq_t)0;

    /* 1 is always refactorable. */
    if (n == 1) return (seq_t)1;

    /* Start with an upper bound that should cover at least the first n refactorables. */
    idx_t limit = n * 25;              /* rough upper bound */
    if (limit < 200) limit = 200;      /* safety for very small n */

    while (1) {
        /* Allocate arrays for smallest prime factor, exponent of that prime, and tau. */
        idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
        idx_t *exp = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
        seq_t *tau = (seq_t *)malloc((limit + 1) * sizeof(seq_t));

        if (!spf || !exp || !tau) {
            /* Allocation failure – clean up and return 0. */
            free(spf); free(exp); free(tau);
            return (seq_t)0;
        }

        /* Initialise spf array. */
        for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
        spf[1] = 1;

        /* Sieve to fill smallest prime factors. */
        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                if (i * i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        if (spf[j] == 0) spf[j] = i;
                }
            }
        }

        /* Initialise tau[1] and exp[1]. */
        tau[1] = (seq_t)1;
        exp[1] = 0;

        idx_t count = 1;   /* 1 is already counted */

        /* Compute tau for each integer and check refactorable condition. */
        for (idx_t i = 2; i <= limit; ++i) {
            idx_t p  = spf[i];
            idx_t j  = i / p;

            if (spf[j] == p) {              /* same prime factor continues */
                exp[i] = exp[j] + 1;
                seq_t denom = (seq_t)(exp[j] + 1);
                seq_t numer = (seq_t)(exp[i] + 1);
                tau[i] = (tau[j] / denom) * numer;
            } else {                         /* new prime factor */
                exp[i] = 1;
                tau[i] = tau[j] * ((seq_t)2);
            }

            if (i % tau[i] == 0) {
                ++count;
                if (count == n) {
                    seq_t result = (seq_t)i;
                    free(spf); free(exp); free(tau);
                    return result;
                }
            }
        }

        /* Not enough refactorables found – increase the upper bound. */
        free(spf); free(exp); free(tau);
        limit <<= 1;            /* double the limit */
        if (limit == 0) limit = (idx_t)-1;   /* fallback for overflow */
    }
}
