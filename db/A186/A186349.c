/*
 * A186349  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=8i and g(j)=j^2. Complement of
 *          A186348.
 * https://oeis.org/A186349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n^2 - 1)/8)
 * Range:     u8 n <= 15, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t square = nn * nn;
    seq_t adj = (square - 1) / 8;
    return nn + adj;
}
