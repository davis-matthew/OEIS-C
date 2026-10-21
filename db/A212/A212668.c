/*
 * A212668  a(n) = (16/3)*(n+1)*n*(n-1) + 8*n^2 + 1.
 * https://oeis.org/A212668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16/3)*(n+1)*n*(n-1) + 8*n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 929, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = (N + 1) * N * (N - 1);   /* product of three consecutive integers */
    seq_t part = (t / 3) * 16;          /* t is always divisible by 3 */
    return part + 8 * N * N + 1;
}
