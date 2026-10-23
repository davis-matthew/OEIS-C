/*
 * A119651  Number of different values of exactly n standard American coins
 *          (pennies, nickels, dimes and quarters).
 * https://oeis.org/A119651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*(n-3) for n > 6 a(n) = 4, 10, 20, 34, 52, 73 for 1 ≤ n
 *            ≤ 6 (These values are the only ones needed before the linear
 *            growth.) The recurrence from OEIS is inaccurate for n < 7, so we
 *            use the explicit values directly
 * Range:     u8 n <= 13, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) through a(6) */
    static const seq_t small[6] = {4, 10, 20, 34, 52, 73};

    if (n <= 6)
        return small[n - 1];
    /* n > 6 */
    seq_t nn = (seq_t)n;
    return (__typeof__(nn))24 * (nn - 3);
}
