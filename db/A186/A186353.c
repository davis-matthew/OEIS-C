/*
 * A186353  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the odd numbers and the
 *          triangular numbers. Complement of A186353.
 * https://oeis.org/A186353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n^2 + n + 1) / 4) // OEIS A186353
 * Range:     u8 n <= 15, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t numerator = nn * nn + nn + (seq_t)1;
    seq_t fraction = numerator / (seq_t)4;
    return nn + fraction;
}
