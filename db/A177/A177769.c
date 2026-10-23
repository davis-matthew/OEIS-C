/*
 * A177769  a(n) = 111*n.
 * https://oeis.org/A177769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 111*n
 * Range:     u8 n <= 2, u16 n <= 590, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * ((seq_t)111);
}
