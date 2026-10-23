/*
 * A040876  Continued fraction for sqrt(907).
 * https://oeis.org/A040876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(907) (OEIS A040876) */
/* a(0) = floor(sqrt(907)) = 30.  For n>0, use the standard
   recurrence:
        m_{k+1} = d_k*a_k - m_k
        d_{k+1} = (907 - m_{k+1}^2) / d_k
        a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})  */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t a0 = (seq_t)30;          /* floor(sqrt(907)) */
    if (n == 0) return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t cur_a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* m_{k+1} = d_k*a_k - m_k */
        m = d * cur_a - m;
        /* d_{k+1} = (907 - m_{k+1}^2) / d_k */
        seq_t m2 = m * m;
        d = ((seq_t)907 - m2) / d;
        /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
        cur_a = (a0 + m) / d;
    }
    return cur_a;
}
