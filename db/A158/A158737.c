/*
 * A158737  a(n) = 1296*n^2 - 36.
 * https://oeis.org/A158737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1296*n^2 - 36
 * Range:     u8 none, u16 n <= 7, u32 n <= 1820, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t val = (seq_t)1296 * ns * ns;
    val -= (seq_t)36;
    return val;
}
