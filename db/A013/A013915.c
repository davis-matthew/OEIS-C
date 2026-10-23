/*
 * A013915  a(n) = F(n) + L(n) + n, where F(n) (A000045) and L(n) (A000204)
 *          are Fibonacci and Lucas numbers respectively.
 * https://oeis.org/A013915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A013915:  a(n) = F(n) + L(n) + n
 * where Fibonacci F(0)=1, F(1)=1 and Lucas L(0)=2, L(1)=1.
 * Recurrence:  a(0)=3,  a(1)=3,
 *              a(n)=a(n-1)+a(n-2)-n+3   (n≥2)
 */

#include <stdint.h>   /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)3;

    seq_t a_prev_prev = (seq_t)3; /* a(0) */
    seq_t a_prev      = (seq_t)3; /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;      /* convert loop index to seq_t */
        a_cur = a_prev + a_prev_prev - i_seq + (seq_t)3;
        a_prev_prev = a_prev;
        a_prev = a_cur;
    }
    return a_prev;
}
