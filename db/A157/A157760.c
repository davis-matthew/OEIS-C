/*
 * A157760  a(n) = 2809*n^2 - 1000*n + 89.
 * https://oeis.org/A157760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2809*n^2 - 1000*n + 89
 * Range:     u8 none, u16 n <= 5, u32 n <= 1236, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)2809 * sn * sn - (seq_t)1000 * sn + (seq_t)89;
}
