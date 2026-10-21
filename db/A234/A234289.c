/*
 * A234289  E.g.f. satisfies: A(x) = 1 + A(x)^2 * Integral 1/A(x) dx.
 * https://oeis.org/A234289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A234289 satisfies A(x) = 1 + A(x)^2 * ∫(1/A(x)) dx. *
 *            Recurrence: a(0)=1, a(1)=1. * For n≥2: * a(n) = a(n-1) +
 *            Σ_{k=0}^{n-2} binom(n-1,k) * a(n-1-k) * (a_k + a_{k+1}) * This
 *            follows from A' = A^2/(2-A) and using e.g.f. coefficients
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *values = NULL;   /* cache of computed terms   */
    static idx_t capacity = 0;     /* allocated size            */
    static idx_t computed = 0;     /* next index to compute     */

    /* extend cache if necessary */
    if (n >= capacity) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *new_values = (seq_t *)realloc(values,
                                              (size_t)new_cap * sizeof(seq_t));
        if (!new_values) {        /* unable to allocate – return 0 */
            return 0;
        }
        values = new_values;
        for (idx_t i = capacity; i < new_cap; ++i) values[i] = 0;
        capacity = new_cap;
    }

    /* already computed */
    if (n < computed)
        return values[n];

    /* initialise if first call */
    if (computed == 0) {
        values[0] = 1;
        computed = 1;
    }

    /* compute terms up to n */
    for (idx_t m = computed; m <= n; ++m) {
        if (m == 0) continue;   /* a(0) already set */
        if (m == 1) {            /* a(1) = a(0) */
            values[1] = values[0];
            continue;
        }

        idx_t nm1 = m - 1;           /* n-1 in recurrence     */
        seq_t sum   = 0;             /* Σ part                */
        seq_t bin   = 1;             /* binom(n-1,0)          */

        for (idx_t k = 0; k <= nm1 - 1; ++k) {
            /* inner = a_k + a_{k+1} */
            seq_t inner = values[k] + values[k + 1];

            /* prod = a_{nm1-k} * inner * binom(n-1,k) */
            seq_t prod = values[nm1 - k] * inner;
            prod = prod * bin;
            sum += prod;

            /* update binomial coefficient for next k */
            if (k < nm1 - 1) {
                /* binom(n-1, k+1) = binom(n-1, k) * (n-1-k) / (k+1) */
                seq_t num = (seq_t)(nm1 - k);
                seq_t den = (seq_t)(k + 1);
                bin = (bin * num) / den;
            }
        }

        values[m] = values[m - 1] + sum;
    }

    computed = n + 1;
    return values[n];
}
