/*
 * A239265  Number of domicule tilings of a 3 X 2n grid.
 * https://oeis.org/A239265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 21*a(n-2) - 3*a(n-3); Initial values: a(0) =
 *            1, a(1) = 5, a(2) = 43
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)43;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)5;
    seq_t a2 = (seq_t)43;
    seq_t an;
    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)13 * a2
           - (seq_t)21 * a1
           - (seq_t)3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
