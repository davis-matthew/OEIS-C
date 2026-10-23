/*
 * A115164  a(n) = 3*a(n-1) + 4*a(n-2), with a(0) = 3, a(1) = 7, a(3) = 9, for
 *          n > 2.
 * https://oeis.org/A115164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) (n > 2) Initial values: a(0) = 3,
 *            a(1) = 7, a(2) = 9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)9;

    /* Start the recurrence from n = 3.
       a_prev    = a(n-1)
       a_prevprev = a(n-2)   */
    seq_t a_prev    = (seq_t)9; /* a(2) */
    seq_t a_prevprev = (seq_t)7; /* a(1) */

    seq_t a_cur;
    for (idx_t i = 3; i <= n; ++i) {
        a_cur = 3 * a_prev + 4 * a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_cur;
    }
    return a_cur;
}
