/*
 * A102396  A mod 2 related Jacobsthal sequence.
 * https://oeis.org/A102396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001045(A000120(n)) (Jacobsthal number of the popcount of
 *            n)
 * Range:     u8 n <= 1022, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute popcount of n */
    idx_t pop = 0;
    idx_t m   = n;
    while (m) {
        pop++;
        m &= m - 1;           /* drop lowest set bit */
    }

    /* compute Jacobsthal number J(pop) */
    if (pop == 0) return (seq_t)0;
    if (pop == 1) return (seq_t)1;

    seq_t J_prev2 = 0; /* J(0) */
    seq_t J_prev1 = 1; /* J(1) */
    for (idx_t i = 2; i <= pop; ++i) {
        seq_t J_cur = J_prev1 + (J_prev2 << 1); /* J(i) = J(i-1) + 2*J(i-2) */
        J_prev2 = J_prev1;
        J_prev1 = J_cur;
    }
    return J_prev1;
}
