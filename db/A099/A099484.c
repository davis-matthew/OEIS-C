/*
 * A099484  A Fibonacci convolution.
 * https://oeis.org/A099484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=3*a(n-1)-2*a(n-2)+3*a(n-3)-a(n-4) with a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)7;

    /* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + 3*a(n-3) - a(n-4). */
    seq_t a_nm4 = (seq_t)1;   /* a(0)   */
    seq_t a_nm3 = (seq_t)1;   /* a(1)   */
    seq_t a_nm2 = (seq_t)2;   /* a(2)   */
    seq_t a_nm1 = (seq_t)7;   /* a(3)   */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)3 * a_nm1
            - (seq_t)2 * a_nm2
            + (seq_t)3 * a_nm3
            - a_nm4;

        /* Shift the window forward. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
