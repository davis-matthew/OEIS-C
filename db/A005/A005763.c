/*
 * A005763  Exponents m_i associated with Weyl group W(E7).
 * https://oeis.org/A005763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1) = 1, a(n) = 2*n + 1 for 2 <= n <= 6, a(7) = 17. These are
 *            the exponents of W(E7): the degrees 2,6,8,10,12,14,18 minus 1.
 *            The sequence is finite
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if (n >= 7) return (seq_t)17;
    return (seq_t)2 * (seq_t)n + 1;
}
