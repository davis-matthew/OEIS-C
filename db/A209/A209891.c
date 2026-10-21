/*
 * A209891  Number of (n+1) X 3 0..2 arrays with every 2X2 subblock having two
 *          distinct values, and new values 0..2 introduced in row major
 *          order.
 * https://oeis.org/A209891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 4*a(n-2) - 16*a(n-3) (n ≥ 4) Initial values:
 *            a(1) = 34, a(2) = 280, a(3) = 2304
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)280;
    if (n == 3) return (seq_t)2304;

    /* a(n-3), a(n-2), a(n-1) respectively */
    seq_t a_n3 = (seq_t)34;
    seq_t a_n2 = (seq_t)280;
    seq_t a_n1 = (seq_t)2304;
    seq_t a_n  = 0;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = a_n1 * 8 + a_n2 * 4 - a_n3 * 16;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = a_n;
    }
    return a_n1;
}
