/*
 * A217053  a(n) = 3*a(n-1) + 24*a(n-2) + a(n-3), with a(0) = 2, a(1) = 5, and
 *          a(2) = 62.
 * https://oeis.org/A217053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 24*a(n-2) + a(n-3), with a(0)=2, a(1)=5,
 *            a(2)=62
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)62;

    /* Recurrence variables (circular buffer of length 3) */
    seq_t a0 = 2, a1 = 5, a2 = 62, a3;

    /* Compute values up to a(n) */
    for (idx_t i = 3; i <= n; ++i) {
        a3 = 3 * a2 + 24 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a3;
}
