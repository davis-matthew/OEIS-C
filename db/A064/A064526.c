/*
 * A064526  Define a pair of sequences by p(0) = 0, q(0) = p(1) = q(1) = 1,
 *          q(n+1) = p(n)*q(n-1), p(n+1) = q(n+1) + q(n) for n > 0; then a(n)
 *          = p(n) and A064183(n) = q(n).
 * https://oeis.org/A064526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure:  Based on the definitions
 *   p(0)=0, p(1)=1, q(0)=q(1)=1
 *   q(n+1) = p(n) * q(n-1)
 *   p(n+1) = q(n+1) + q(n)
 * A(n) = p(n)
 * This function iteratively computes the required terms without storing
 * the whole sequence, using only the two most recent values of p and q.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* a[0], a[1] and q[0], q[1] */
    seq_t prev_a = (seq_t)0;    /* a[n-2]  */
    seq_t curr_a = (seq_t)1;    /* a[n-1]  */
    seq_t prev_q = (seq_t)1;    /* q[n-2]  */
    seq_t curr_q = (seq_t)1;    /* q[n-1]  */

    idx_t i;
    for (i = 2; i <= n; ++i)
    {
        seq_t q_i = curr_a * prev_q;   /* q[n] = a[n-1] * q[n-2] */
        seq_t a_i = q_i + curr_q;      /* p[n] = q[n]   + q[n-1] */

        /* shift indices for next iteration */
        prev_q = curr_q;
        curr_q = q_i;

        prev_a = curr_a;
        curr_a = a_i;
    }
    return curr_a;
}
