/*
 * A233035  a(n) = n * floor(n/4).
 * https://oeis.org/A233035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * floor(n/4)
 * Range:     u8 n <= 31, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t q = nn / ((seq_t)4);
    return nn * q;
}
