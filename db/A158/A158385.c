/*
 * A158385  a(n) = 676*n^2 + 2*n.
 * https://oeis.org/A158385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 676*n^2 + 2*n
 * Range:     u8 none, u16 n <= 9, u32 n <= 2520, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = n;
    return 676 * tn * tn + 2 * tn;
}
