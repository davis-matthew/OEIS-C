/*
 * A073385  Eighth convolution of A000129(n+1) (generalized (2,1)-Fibonacci,
 *          called Pell numbers), n>=0, with itself.
 * https://oeis.org/A073385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 12, u64 n <= 31, u128 n <= 74,
 *            big any n
 * Helpers:   binom_fixed8, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073385: Eighth convolution of Pell numbers A000129(n+1) with itself.
   Formula: a(n) = Σ_{k=0..⌊n/2⌋} 2^(n-2k) * binomial(n-k+8, 8) * binomial(n-k, k)   */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/binom_fixed8.h"

/* Compute a(n) for any non‑negative n. */
seq_t a(idx_t n) {
    /* Special case: n = 0 works naturally below, but guard for speed */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Number of terms in the summation: floor(n/2) */
    idx_t maxk = n >> 1;          /* efficient floor(n/2) */

    /* 2^(n-2k) – start with 2^n and divide by 4 at each step */
    seq_t pow_term = pow2(n);

    /* B_k = binomial(n-k, k); start with k = 0 */
    seq_t B = 1;

    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial(n-k+8, 8) */
        seq_t binom_nk8 = binom_fixed8(n - k + 8);

        /* term = pow_term * binom_nk8 * B */
        seq_t term = pow_term * binom_nk8;
        term = term * B;
        sum += term;

        /* Prepare for next iteration */
        if (k < maxk) {
            /* Update B to binomial(n-(k+1), k+1) */
            /* B_next = B * (n-2k) * (n-2k-1) / ((k+1) * (n-k)) */
            seq_t num1 = (seq_t)(n - 2 * k);
            seq_t num2 = (seq_t)(n - 2 * k - 1);
            seq_t denom = (seq_t)(k + 1) * (seq_t)(n - k);

            B = B * num1;
            B = B * num2;
            B = B / denom;

            /* Move power term down by factor of 4 (divide by 2^2) */
            pow_term /= (seq_t)4;
        }
    }

    return sum;
}
