/*
 * A158220  a(n) = 169*n^2 + 2*n.
 * https://oeis.org/A158220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 169*n^2 + 2*n
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 5041, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return ((seq_t)169) * m * m + ((seq_t)2) * m;
}
