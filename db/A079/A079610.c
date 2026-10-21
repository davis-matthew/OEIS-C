/*
 * A079610  a(n) = (5*n+1)*(5*n+3)*(5*n+5).
 * https://oeis.org/A079610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n+1)*(5*n+3)*(5*n+5)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 324, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t1 = (seq_t)5 * n + 1;
    seq_t t2 = (seq_t)5 * n + 3;
    seq_t t3 = (seq_t)5 * n + 5;
    return t1 * t2 * t3;
}
