/*
 * A081271  Vertical of triangular spiral in A051682.
 * https://oeis.org/A081271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 15*n + 2)/2, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t value = (9 * nn * nn + 15 * nn + 2);
    value /= 2;          /* integer division */
    return value;
}
