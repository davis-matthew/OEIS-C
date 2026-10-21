/*
 * A240137  Sum of n consecutive cubes starting from n^3.
 * https://oeis.org/A240137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (3*n - 1) * (5*n - 3) / 4
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 183, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of n consecutive cubes starting from n^3, OEIS A240137 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;                 /* n^2 */
    seq_t t1 = 3 * sn - (seq_t)1;       /* 3*n - 1 */
    seq_t t2 = 5 * sn - (seq_t)3;       /* 5*n - 3 */
    return (n2 * t1 * t2) / (seq_t)4;
}
