/*
 * A095129  a(n) = Jac(n)*(2Jac(n)-1), where Jac(n) = A001045(n).
 * https://oeis.org/A095129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Jac(n) * (2 * Jac(n) - 1) where Jac(n) (Jacobsthal
 *            numbers) satisfies J(0) = 0, J(1) = 1, J(n) = J(n-1) + 2 *
 *            J(n-2) for n ≥ 2. This implementation computes the required a(n)
 *            directly using only integer arithmetic suitable for 64‑bit,
 *            128‑bit or arbitrary‑precision seq_t
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;        /* a(0) = 0 */

    /* Compute Jacobsthal number J(n) iteratively */
    seq_t J_prev2 = (seq_t)0;            /* J(0) */
    seq_t J_prev1 = (seq_t)1;            /* J(1) */

    if (n == 1u) return (seq_t)1;        /* a(1) = J(1)*(2J(1)-1) = 1 */

    seq_t J_curr = J_prev1;              /* placeholder for J(n) */
    for (idx_t i = 2; i <= n; ++i)
    {
        J_curr = J_prev1 + ((seq_t)2 * J_prev2);
        J_prev2 = J_prev1;
        J_prev1 = J_curr;
    }
    /* J_curr now holds J(n) */

    seq_t twoJ = (seq_t)2 * J_curr;
    return J_curr * (twoJ - (seq_t)1);
}
