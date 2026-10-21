/*
 * A220857  Number of mappings by Struijk et al. Design Space Explorations
 *          with n actors and three implementation alternatives.
 * https://oeis.org/A220857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 30*n^2 - 27*n for n >= 4; with initial terms a(1) = 3,
 *            a(2) = 42, a(3) = 180
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)42;
    if (n == 3) return (seq_t)180;
    seq_t nn = (seq_t)n;
    seq_t tmp = (seq_t)(30 * nn - 27);
    return nn * tmp;
}
