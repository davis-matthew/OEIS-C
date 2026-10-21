/*
 * A210397  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          one or three distinct values, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A210397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 5*a(n-2) - 4*a(n-3) Initial values: a(1)=7,
 *            a(2)=59, a(3)=549
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial three terms explicitly */
    if (n == (idx_t)1) return (seq_t)7;
    if (n == (idx_t)2) return (seq_t)59;
    if (n == (idx_t)3) return (seq_t)549;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)7;          /* a(1)   */
    seq_t a1 = (seq_t)59;         /* a(2)   */
    seq_t a2 = (seq_t)549;        /* a(3)   */

    /* Iterate the recurrence from n = 4 up to the requested n */
    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        seq_t next = 10 * a2;     /* 10*a(n-1)  */
        next -= 5 * a1;          /* -5*a(n-2)  */
        next -= 4 * a0;          /* -4*a(n-3)  */

        /* Shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2; /* a(n) */
}
