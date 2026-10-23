/*
 * A158636  a(n) = 576*n^2 - 24.
 * https://oeis.org/A158636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 576*n^2 - 24
 * Range:     u8 none, u16 n <= 10, u32 n <= 2730, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The calculation is performed in type seq_t to avoid intermediate
     * overflow. Casting the constants to seq_t ensures that the
     * multiplications use the proper width.
     */
    seq_t nn = (seq_t)n;
    return (seq_t)576 * nn * nn - (seq_t)24;
}
