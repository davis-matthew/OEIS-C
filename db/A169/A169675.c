/*
 * A169675  Lexicographically earliest de Bruijn sequence for n = 3 and k = 2.
 * https://oeis.org/A169675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[n mod 8], where pattern = 00010111
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t seq[8] = {0, 0, 0, 1, 0, 1, 1, 1};
    return seq[n & 7];
}
