/*
 * A054410  Susceptibility series H_3 for 2-dimensional Ising model (divided
 *          by 2).
 * https://oeis.org/A054410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 972, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A054410
   Formula:  a(0) = 1
             a(n) = 2*n*(11 + 7*n^2)/3   for n > 0
   This direct formula reproduces the generating function
   (1 +8*x +10*x^2 +8*x^3 +x^4)/(1-x)^4 and the known initial
   terms.  It evaluates in O(1) arithmetic operations using only
   integer operators permitted by the build configuration. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 1;

    seq_t n_s   = (seq_t)n;           /* cast index to sequence type   */
    seq_t n_sq  = n_s * n_s;          /* n^2                            */
    seq_t term  = (seq_t)7 * n_sq;    /* 7*n^2                          */
    term += (seq_t)11;                /* 11 + 7*n^2                     */

    /* Divide by 3 as early as possible to keep intermediate values
       from overflowing for 64-bit builds.  Because 3 always divides
       the numerator, we can safely split the division. */
    if ((n_s % (seq_t)3) == 0) {
        /* n is a multiple of 3, so divide n by 3 first */
        seq_t n_div3 = n_s / (seq_t)3;
        return (seq_t)2 * n_div3 * term;
    } else {
        /* (11 + 7*n^2) is divisible by 3 in this case. */
        term = term / (seq_t)3;
        return (seq_t)2 * n_s * term;
    }
}
