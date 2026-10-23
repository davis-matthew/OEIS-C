/*
 * A170802  a(n) = n^10*(n^10 + 1)/2.
 * https://oeis.org/A170802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10*(n^10 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n10 = 1;
    for (int i = 0; i < 10; ++i)
        n10 *= (seq_t)n;
    return n10 * (n10 + (seq_t)1) / ((seq_t)2);
}
