/*
 * A074356  Coefficient of q^2 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,3).
 * https://oeis.org/A074356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A074356: coefficient of q^2 in nu(n) with
 *   nu(0) = 1, nu(1) = 1 and for n ≥ 2
 *     nu(n) = nu(n-1) + 3 * (1 + q + ... + q^(n-2)) * nu(n-2)
 * Recurrence for the three lowest coefficients:
 *   a0[n] = a0[n-1] + 3 * a0[n-2]
 *   a1[n] = a1[n-1] + 3 * (a1[n-2] + a0[n-2])      (for n ≥ 3, otherwise a1[n]=a1[n-1])
 *   a2[n] = a2[n-1] + 3 * (a2[n-2] + a1[n-2] + a0[n-2])
 *   with a0[0] = 1, a1[0] = 0, a2[0] = 0
 *       a0[1] = 1, a1[1] = 0, a2[1] = 0
 *       a0[2] = 4, a1[2] = 0, a2[2] = 0
 *       a0[3] = 7, a1[3] = 3, a2[3] = 0
 * The function returns a2[n].
 */

#include <stdlib.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* static arrays for the three lowest coefficients */
    static seq_t *a0 = NULL, *a1 = NULL, *a2 = NULL;
    static size_t cap = 0;          /* allocated size of the arrays       */
    static idx_t last = 0;          /* highest index already computed     */

    /* Ensure capacity for indices up to n */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        a0 = (seq_t*)realloc(a0, newcap * sizeof(seq_t));
        a1 = (seq_t*)realloc(a1, newcap * sizeof(seq_t));
        a2 = (seq_t*)realloc(a2, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialise base values up to index 3 if not yet done */
    if (last < 3) {
        a0[0] = 1;   a1[0] = 0;   a2[0] = 0;
        a0[1] = 1;   a1[1] = 0;   a2[1] = 0;
        a0[2] = 4;   a1[2] = 0;   a2[2] = 0;
        a0[3] = 7;   a1[3] = 3;   a2[3] = 0;
        last = 3;
    }

    /* Compute missing indices up to n */
    for (size_t i = (size_t)(last + 1); i <= (size_t)n; ++i) {
        /* a0 recurrence */
        seq_t a0_i = a0[i-1] + (seq_t)3 * a0[i-2];

        /* a1 recurrence – add the extra term only for i ≥ 3 */
        seq_t a1_i = a1[i-1];
        if (i-2 >= 1) {
            a1_i += (seq_t)3 * (a1[i-2] + a0[i-2]);
        }

        /* a2 recurrence – include terms that exist */
        seq_t sum2 = a2[i-2];
        if (i-2 >= 1) sum2 += a1[i-2];
        if (i-2 >= 2) sum2 += a0[i-2];
        seq_t a2_i = a2[i-1] + (seq_t)3 * sum2;

        /* Store results for this i */
        a0[i] = a0_i;
        a1[i] = a1_i;
        a2[i] = a2_i;
    }
    last = n;

    /* Return the coefficient of q^2 */
    return a2[(size_t)n];
}
