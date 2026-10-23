/*
 * A153372  Number of zig-zag paths from top to bottom of a rectangle of width
 *          11 with 2n-1 rows whose color is that of the top right corner.
 * https://oeis.org/A153372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3) with initial values a(1) =
 *            6, a(2) = 20, a(3) = 72
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)72;

    seq_t a_n3 = (seq_t)6;   /* a(1) */
    seq_t a_n2 = (seq_t)20;  /* a(2) */
    seq_t a_n1 = (seq_t)72;  /* a(3) */
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)6 * a_n1 - (seq_t)9 * a_n2 + (seq_t)2 * a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = a_n;
    }
    return a_n1;
}
