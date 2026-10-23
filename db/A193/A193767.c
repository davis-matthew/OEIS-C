/*
 * A193767  The number of dominoes in a largest saturated domino covering of
 *          the 4 by n board.
 * https://oeis.org/A193767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3n, except for n = 1,2,3,5,6 or 9 where a(n) = 3n-1. For
 *            n > 11, a(n) satisfies the linear recurrence a(n) = 2*a(n-1) -
 *            a(n-2). Initial values: a(1)=2, a(2)=5, a(3)=8, a(4)=12,
 *            a(5)=14, a(6)=17, a(7)=21, a(8)=24, a(9)=26, a(10)=30, a(11)=33
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* below offset – undefined */

    /* small n handled directly by the closed form */
    if (n <= 11) {
        if (n == 1 || n == 2 || n == 3 || n == 5 || n == 6 || n == 9)
            return (seq_t)3 * (seq_t)n - 1;
        return (seq_t)3 * (seq_t)n;
    }

    /* recurrence based on a(10)=30 and a(11)=33 */
    seq_t a_prev = (seq_t)3 * (seq_t)10;        /* a(10) */
    seq_t a_curr = (seq_t)3 * (seq_t)11;        /* a(11) */

    for (idx_t i = 12; i <= n; ++i) {
        seq_t next = (seq_t)2 * a_curr - a_prev;
        a_prev = a_curr;
        a_curr = next;
    }
    return a_curr;
}
