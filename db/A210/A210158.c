/*
 * A210158  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          two or three distinct values, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A210158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 16*a(n-1) - 19*a(n-2) - 80*a(n-3) - 36*a(n-4) * initial
 *            values: * a(1) = 13, a(2) = 148, a(3) = 1977, a(4) = 27720
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* outside the offset, not used */
    switch (n) {
        case 1: return (seq_t)13;
        case 2: return (seq_t)148;
        case 3: return (seq_t)1977;
        case 4: return (seq_t)27720;
    }

    /* variables holding a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a1 = (seq_t)13;    /* a(n-4)  for current iteration */
    seq_t a2 = (seq_t)148;   /* a(n-3)  */
    seq_t a3 = (seq_t)1977;  /* a(n-2)  */
    seq_t a4 = (seq_t)27720; /* a(n-1)  */
    seq_t cur = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        /* recurrence: a(n) = 16*a(n-1) - 19*a(n-2) - 80*a(n-3) - 36*a(n-4) */
        cur = 16 * a4 - 19 * a3 - 80 * a2 - 36 * a1;
        /* shift for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return cur;
}
