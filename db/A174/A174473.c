/*
 * A174473  G.f.: exp( Sum_{n>=1} A174937(n)*x^n/n ) where A174937(n) =
 *          Sum_{d|n} d^tau(d).
 * https://oeis.org/A174473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Sum_{k>=1} R(k) * x^k / k ), where * a(0)=1 and R(k)
 *            = Sum_{d|k} d^{tau(d)} (tau(d) = number of divisors of d). *
 *            Recurrence: a(n) = (1/n) * Sum_{k=1}^n k*R(k)*a(n-k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* static tables */
    static idx_t cap = 0;                 /* allocated size */
    static seq_t *aVals = NULL;           /* a values  */
    static seq_t *rVals = NULL;           /* R values */
    static uint32_t *tau = NULL;          /* tau values */
    static idx_t computed_upto = 0;       /* highest n computed for tau & r */
    static idx_t computed_a_upto = 0;     /* highest n computed for a */

    /* allocate or resize arrays as needed */
    if (n + 1 > cap) {
        /* free old memory */
        if (aVals) free(aVals);
        if (rVals) free(rVals);
        if (tau)  free(tau);

        /* allocate new size */
        cap = n + 1;
        aVals = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        rVals = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        tau   = (uint32_t *)calloc((size_t)cap, sizeof(uint32_t));

        computed_upto = 0;          /* will be recomputed below */
        computed_a_upto = 0;        /* will be recomputed below */
    }

    /* recompute tau and R if a larger n is needed */
    if (n > computed_upto) {
        /* zero tau and rVals up to n */
        for (idx_t i = 1; i <= n; ++i) {
            tau[i] = 0;
            rVals[i] = 0;
        }

        /* compute tau[1..n] */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                ++tau[j];
            }
        }

        /* compute rVals[1..n] */
        for (idx_t d = 1; d <= n; ++d) {
            /* pow = d^{tau[d]} */
            seq_t pow = (seq_t)1;
            uint32_t e;
            for (e = 0; e < tau[d]; ++e) {
                pow *= (seq_t)d;
            }

            /* add pow to all multiples of d */
            for (idx_t m = d; m <= n; m += d) {
                rVals[m] += pow;
            }
        }

        computed_upto = n;
    }

    /* compute a[ ] up to n if needed */
    if (n > computed_a_upto) {
        aVals[0] = (seq_t)1;   /* a(0) = 1 */
        for (idx_t cur = computed_a_upto + 1; cur <= n; ++cur) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= cur; ++k) {
                sum += rVals[k] * aVals[cur - k];
            }
            aVals[cur] = sum / (seq_t)cur;
        }
        computed_a_upto = n;
    }

    return aVals[n];
}
