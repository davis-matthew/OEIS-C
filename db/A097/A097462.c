/*
 * A097462  Multiplication table for numbers 0 through 10 read by rows.
 * https://oeis.org/A097462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = floor(n/11) * (n mod 11)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n / 11;
    idx_t c = n % 11;
    return (seq_t)r * (seq_t)c;
}
