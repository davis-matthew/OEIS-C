/*
 * A116731  Number of permutations of length n which avoid the patterns 321,
 *          2143, 3124; or avoid the patterns 132, 2314, 4312, etc.
 * https://oeis.org/A116731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - 3*n^2 + 5*n)/3
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t temp = nt * nt - 3 * nt + 5;   /* n^2 - 3n + 5 */
    return nt * temp / 3;                /* n * (n^2 - 3n + 5) / 3 */
}
