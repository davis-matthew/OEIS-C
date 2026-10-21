/*
 * A057666  a(n) = n*(n+1)^2*(n+2)*(n+3)^2*(n+4).
 * https://oeis.org/A057666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)*(n+3)^2*(n+4)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 21, u64 n <= 563,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x * (x + 1) * (x + 1) * (x + 2) * (x + 3) * (x + 3) * (x + 4);
}
