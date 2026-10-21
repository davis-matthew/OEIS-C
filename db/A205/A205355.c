/*
 * A205355  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          the same number of equal diagonal or antidiagonal elements, and
 *          new values 0..2 introduced in row major order.
 * https://oeis.org/A205355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 76*a(n-2) + 146*a(n-3) - 84*a(n-4); Initial
 *            values: a(1)=50, a(2)=275, a(3)=1562, a(4)=8948 (Empirical
 *            recurrence from OEIS)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, return 0 */

    if (n == 1) return 50;
    if (n == 2) return 275;
    if (n == 3) return 1562;
    if (n == 4) return 8948;

    seq_t a1 = 50;   /* a(n-4) */
    seq_t a2 = 275;  /* a(n-3) */
    seq_t a3 = 1562; /* a(n-2) */
    seq_t a4 = 8948; /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i)
    {
        next = 15 * a4 - 76 * a3 + 146 * a2 - 84 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
