/*
 * A124351  Order of the automorphism group of the n-prism graph.
 * https://oeis.org/A124351
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3) = 12, a(4) = 48, otherwise a(n) = 4*n for n ≥ 5
 * Range:     u8 n <= 63, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)48;
    if (n >= 5) return (seq_t)n * (seq_t)4;
    /* for n < 3 the offset is not defined – return 0 */
    return (seq_t)0;
}
