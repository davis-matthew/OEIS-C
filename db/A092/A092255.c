/*
 * A092255  a(0) = 1; for n > 0, a(n) = b(n) - n*b(n-1), b() = A076177().
 * https://oeis.org/A092255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} C(n,k) * Σ_{j=k}^{⌊(n−k)/2⌋} C(n−k , j)
 *            (C(m,t) = multinomial coefficient = m! / (t!(m−t)!)) Implements
 *            the combinatorial interpretation of sequence A092255
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   binom_unsigned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=10, ... */

#include <stdint.h>
#include "../utils/binom_unsigned.h"

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0 || n == 1) return (seq_t)1;

    seq_t sum = 0;
    seq_t binom_n_k = 1;          /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = n - k;          /* remaining positions after choosing k zeros */

        /* Require k ≤ j ≤ m/2 ⇒ m ≥ 2*k */
        if (m < (idx_t)2 * k) break;   /* for larger k the condition fails */

        idx_t j_max = m / 2;        /* floor((n−k)/2) */
        /* Start with j = k */
        seq_t binom_m_j = binom_unsigned(m, k);  /* C(m, k) */

        for (idx_t j = k; j <= j_max; ++j) {
            /* term = C(n,k) * C(m,j) */
            sum += binom_n_k * binom_m_j;

            /* Prepare next binomial C(m, j+1) if not at last j */
            if (j < j_max) {
                binom_m_j = binom_m_j * ((seq_t)m - j) / (seq_t)(j + 1);
            }
        }

        /* Update binom_n_k for next k (C(n, k+1)) */
        if (k < n) {
            binom_n_k = binom_n_k * ((seq_t)n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
