/*
 * A156855  a(n) = 2025*n^2 - n.
 * https://oeis.org/A156855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2025*n^2 - n
 * Range:     u8 none, u16 n <= 5, u32 n <= 1456, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = n;
    return (seq_t)2025 * nt * nt - nt;
}
