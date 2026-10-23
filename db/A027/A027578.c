/*
 * A027578  Sums of five consecutive squares: a(n) = n^2 + (n+1)^2 + (n+2)^2 +
 *          (n+3)^2 + (n+4)^2.
 * https://oeis.org/A027578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*n + 20*n + 30
 * Range:     u8 n <= 5, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 5 * nn * nn + 20 * nn + 30;
}
