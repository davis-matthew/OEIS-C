/*
 * A234971  a(n) = Sum_{k=0..n} n^k * binomial(n,k)^4.
 * https://oeis.org/A234971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(n,k)^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Compute the sum a(n) = Σ_{k=0..n} n^k * (C(n,k))^4 using an
       iterative formula for binomial coefficients and powers of n.
       This runs in O(n) time with only integer arithmetic. */
    seq_t sum   = 0;          /* running total of the sum      */
    seq_t coeff = 1;          /* C(n,0) = 1                    */
    seq_t pow   = 1;          /* n^0 = 1                        */
    seq_t n_seq = (seq_t)n;   /* cast n to seq_t for arithmetic */

    for (idx_t k = 0; k <= n; ++k) {
        /* coeff^4 = coeff^2 * coeff^2  */
        seq_t coeff_sq       = coeff * coeff;
        seq_t coeff_quartic  = coeff_sq * coeff_sq;

        /* term = n^k * coeff^4  */
        seq_t term = pow * coeff_quartic;
        sum      += term;

        if (k == n) break;        /* finished the last term */

        /* update pow   : n^{k+1} = n^k * n  */
        pow = pow * n_seq;

        /* update coeff : C(n,k+1) = C(n,k) * (n-k)/(k+1)  */
        coeff = coeff * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return sum;
}
