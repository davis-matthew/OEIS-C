/*
 * A133899  Numbers m such that binomial(m+9,m) mod 9 = 0.
 * https://oeis.org/A133899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*n + 72 - 8*(n mod 9)
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)(n % 9);
    return (seq_t)n * 9 + 72 - 8 * mod;
}
