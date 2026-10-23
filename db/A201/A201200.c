/*
 * A201200  Total number of round trips of length n on the closed Laguerre
 *          graph Lc_4 divided by 4.
 * https://oeis.org/A201200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 16*a(n-1)-68*a(n-2)+64*a(n-3)+44*a(n-4), with a(0)=1,
 *            a(1)=4, a(2)=30, a(3)=256
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)30;
    if (n == 3) return (seq_t)256;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)30;
    seq_t a3 = (seq_t)256;
    seq_t newval;

    for (idx_t i = 4; i <= n; ++i) {
        newval = (seq_t)16 * a3 - (seq_t)68 * a2 + (seq_t)64 * a1 + (seq_t)44 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = newval;
    }
    return a3;
}
