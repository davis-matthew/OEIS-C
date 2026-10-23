/*
 * A153371  Number of zig-zag paths from top to bottom of a rectangle of width
 *          11 with 2n rows whose color is that of the top right corner.
 * https://oeis.org/A153371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3) for n>=4, with a(1)=10,
 *            a(2)=36, a(3)=132
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n<1, dummy value */
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)36;
    if (n == 3) return (seq_t)132;

    seq_t a1 = (seq_t)10;
    seq_t a2 = (seq_t)36;
    seq_t a3 = (seq_t)132;
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)6 * a3 - (seq_t)9 * a2 + (seq_t)2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
