/*
 * A084065  Fourth row of number array A084061.
 * https://oeis.org/A084065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n - sqrt(3))^n + (n + sqrt(3))^n)/2 Equivalent integer
 *            form: a(n) = Σ_{m=0..⌊n/2⌋} binom(n, 2m) * n^{n-2m} * 3^m
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT */

#include <stdint.h>
#include "../utils/seq_pow_big.h"

seq_t a(idx_t n) {
    if (n == 0u) return 1;
    if (n == 1u) return 1;

    /* compute n^n */
    seq_t term = seq_pow_big((seq_t)n, n);      /* term = n^n       */
    seq_t sum  = term;                          /* start sum       */
    seq_t coeff = 1;                            /* binom(n,0)      */
    seq_t two_n_sq = (seq_t)n * (seq_t)n;       /* n^2             */

    idx_t max_m = n >> 1;                       /* floor(n/2)      */

    for (idx_t m = 1; m <= max_m; ++m) {
        /* Update binomial coefficient: binom(n, 2m) from binom(n, 2(m-1)) */
        idx_t k_prev = 2*(m-1);                 /* previous k      */
        idx_t k = 2*m;                          /* current k       */
        coeff = coeff * (seq_t)(n - k_prev) * (seq_t)(n - k_prev - 1);
        coeff = coeff / ((seq_t)k * (seq_t)(k - 1));

        /* Update term: n^{n-2m} * 3^m from previous term */
        term = term / two_n_sq;                 /* divide by n^2    */
        term = term * 3;                       /* multiply by 3    */

        /* Accumulate contribution */
        sum += coeff * term;
    }

    return sum;
}
