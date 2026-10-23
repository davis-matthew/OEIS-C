/*
 * A132117  Binomial transform of [1, 7, 17, 17, 6, 0, 0, 0, ...].
 * https://oeis.org/A132117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n^2 + n + 2)/12
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 232, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; a(0) is unused but defined as 0 */
    seq_t N = (seq_t)n;
    seq_t t1 = N * (N + 1);                             /* n*(n+1) */
    seq_t t2 = (seq_t)3 * N * N + N + 2;                /* 3*n^2 + n + 2 */
    return (t1 * t2) / 12;                              /* integer division, divisible by 12 */
}
