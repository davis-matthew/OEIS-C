/*
 * A135639  a(n) = 839*n.
 * https://oeis.org/A135639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 839*n
 * Range:     u8 n <= 0, u16 n <= 78, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t coef = (seq_t)839;
    return (seq_t)n * coef;
}
