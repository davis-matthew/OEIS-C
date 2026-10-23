/*
 * A192447  a(n) = n*(n-1)/2 if this is even, otherwise (n*(n-1)/2) + 1.
 * https://oeis.org/A192447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n-1)/2) if even, else (n*(n-1)/2)+1
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t tri = tn * (tn - 1) / 2;
    return tri + (tri % 2);
}
