/*
 * A158401  a(n) = 841*n^2 - 2*n.
 * https://oeis.org/A158401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 841*n^2 - 2*n
 * Range:     u8 none, u16 n <= 8, u32 n <= 2259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t sn = (seq_t)n;
    return (seq_t)841 * sn * sn - (seq_t)2 * sn;
}
