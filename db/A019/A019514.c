/*
 * A019514  a(n) = (n!)^3 + 1.
 * https://oeis.org/A019514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^3 + 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact = fact * (seq_t)i;
    return fact * fact * fact + (seq_t)1;
}
