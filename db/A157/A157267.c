/*
 * A157267  a(n) = 10368*n^2 - 4896*n + 577.
 * https://oeis.org/A157267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10368*n^2 - 4896*n + 577
 * Range:     u8 none, u16 n <= 2, u32 n <= 643, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)10368 * nn * nn - (seq_t)4896 * nn + (seq_t)577;
    return result;
}
