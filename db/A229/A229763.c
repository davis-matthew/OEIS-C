/*
 * A229763  a(n) = (2*n) XOR n AND n, where AND and XOR are bitwise logical
 *          operators.
 * https://oeis.org/A229763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n AND NOT 2n
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) & ~(((seq_t)n) << 1);
}
