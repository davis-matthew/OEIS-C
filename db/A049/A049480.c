/*
 * A049480  a(n) = (2*n-1)*(n^2 -n +6)/6.
 * https://oeis.org/A049480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(n^2 - n + 6)/6
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* sequence undefined for n=0, return 0 */
    seq_t N = (seq_t)n;
    seq_t t1 = (seq_t)2 * N - (seq_t)1;
    seq_t t2 = N * N - N + (seq_t)6;
    seq_t prod = t1 * t2;
    return prod / (seq_t)6;
}
