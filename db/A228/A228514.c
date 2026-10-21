/*
 * A228514  a(n) = Sum_{k=0..n} binomial(n,k)^2*binomial(2k,k+1).
 * https://oeis.org/A228514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(2k,k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum iteratively using the identities */
/*   binom(n,k) = binom(n,k-1) * (n-k+1)/k  (k>=1) */
/*   binom(2k,k+1) = binom(2(k-1),k) * (2k)(2k-1)/((k+1)(k-1))  (k>=2) */
/*  with binom(n,0)=1, binom(2,2)=1. */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;                     /* accumulator for the sum          */
    seq_t binom_n_k = 1;                /* binomial(n,0)                     */
    seq_t B_k = 0;                      /* binomial(2k,k+1) for current k    */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update binomial(n,k) using recurrence */
            binom_n_k = binom_n_k *
                        ((seq_t)(n - k + 1)) /
                        (seq_t)k;

            /* Update binomial(2k,k+1) */
            if (k == 1) {
                B_k = 1;  /* binom(2,2) */
            } else {
                seq_t num = (seq_t)(2 * k) * (seq_t)(2 * k - 1);
                seq_t den = (seq_t)(k + 1) * (seq_t)(k - 1);
                B_k = B_k * num / den;
            }
        }
        /* Add current term to the sum */
        seq_t term = binom_n_k * binom_n_k * B_k;
        sum += term;
    }
    return sum;
}
