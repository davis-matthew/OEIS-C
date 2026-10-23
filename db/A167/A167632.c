/*
 * A167632  Smallest m such that A033183(m) = n.
 * https://oeis.org/A167632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*(n-1) for n >= 3, a(0) = 0, a(1) = 4, a(2) = 36
 * Range:     u8 n <= 8, u16 n <= 1821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)36;
    return ((seq_t)36) * ((seq_t)n - 1);
}
