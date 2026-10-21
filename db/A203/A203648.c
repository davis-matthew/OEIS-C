/*
 * A203648  a(n) = (1/4) * period of repeating sequence {S(j) mod 2n}, where
 *          S(j) is the sum of the first j squares.
 * https://oeis.org/A203648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n if n % 3 != 0, otherwise a(n) = 3 * n
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0)
        return (seq_t)n * 3;
    else
        return (seq_t)n;
}
