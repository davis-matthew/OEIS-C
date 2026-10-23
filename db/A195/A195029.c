/*
 * A195029  a(n) = n*(14*n + 13) + 3.
 * https://oeis.org/A195029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n*n + 13*n + 3
 * Range:     u8 n <= 3, u16 n <= 67, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 14*nn*nn + 13*nn + 3;
}
