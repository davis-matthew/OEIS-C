/*
 * A013926  a(n) = (2*n)! * D_{2*n}, where D_{2*n} = (1/Pi) *
 *          Integral_{x=0..oo} [1 - x^(2*n) / Product_{j=1..n} (x^2+j^2)] dx.
 * https://oeis.org/A013926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ( n^(2n) + Σ_{k=1}^{n-1} (-1)^k * (2/k) * * C(2n-1,
 *            k-1) * (n-k)^(2n+1) )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* sequence offset starts at 1 */

    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    /* k = 0 term (always even) */
    sum_even += big_pow((seq_t)n, (seq_t)(2*n));

    /* iterative binomial C(2n-1, k-1), starting with k = 1 → binom = 1 */
    seq_t binom = 1;

    for (idx_t k = 1; k < n; ++k) {
        /* (n-k)^(2n+1) */
        seq_t power = big_pow((seq_t)(n - k), (seq_t)(2*n + 1));

        /* term = 2/k * binom * power  (division is exact) */
        seq_t term = binom * power * 2;
        term /= k;

        /* accumulate according to parity of k (no negative intermediates) */
        if ((k & 1) == 0)
            sum_even += term;
        else
            sum_odd  += term;

        /* update binom to C(2n-1, k) for next iteration */
        binom = binom * (seq_t)(2*n - k);
        binom /= k;
    }

    /* final sum = even terms – odd terms (sum_even > sum_odd) */
    seq_t sum = sum_even - sum_odd;

    /* multiply by n for the final result */
    return sum * (seq_t)n;
}
