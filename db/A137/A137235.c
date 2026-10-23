/*
 * A137235  a(n) = (n+1)/2 if n is odd; a(n) = n/2 + 6 if n is even.
 * https://oeis.org/A137235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)/2 if n is odd; a(n) = n/2 + 6 if n is even
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 1) {
        return ((seq_t)n + 1) / 2;
    }
    return ((seq_t)n) / 2 + 6;
}
