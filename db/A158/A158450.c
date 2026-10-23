/*
 * A158450  Number of spanning forests in 3 X n grid.
 * https://oeis.org/A158450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*a(n-1) - 154*a(n-2) + 176*a(n-3) - 32*a(n-4) for n >=
 *            5, with initial values: a(0) = 1 a(1) = 4 a(2) = 112 a(3) = 3102
 *            a(4) = 85818 (These match the OEIS sequence A158450.)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)112;
    if (n == 3) return (seq_t)3102;
    if (n == 4) return (seq_t)85818;

    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)4;
    seq_t a3 = (seq_t)112;
    seq_t a4 = (seq_t)3102;
    seq_t a5 = (seq_t)85818;

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = (seq_t)33 * a5 - (seq_t)154 * a4 + (seq_t)176 * a3 - (seq_t)32 * a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
