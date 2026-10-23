/*
 * A158563  a(n) = 32*n^2 - 1.
 * https://oeis.org/A158563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    val = val * val;
    val = val * (seq_t)32;
    val = val - (seq_t)1;
    return val;
}
