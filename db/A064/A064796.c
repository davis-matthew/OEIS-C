/*
 * A064796  Largest integer m such that every permutation (p_1, ..., p_n) of
 *          (1, ..., n) satisfies p_i * p_{i+1} >= m for some i, 1 <= i <= n,
 *          where p_{n+1} = p_1.
 * https://oeis.org/A064796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (n^2 + 4*n + 3)/4 if n is odd, a(n) = (n^2 + 4*n)/4 if n
 *            is even; (special cases: a(1)=1, a(2)=2)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;

    if (n == 1)   /* a(1) = 1 */
        return 1;

    if (n == 2)   /* a(2) = 2 */
        return 2;

    /* cast n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    if (n & 1)    /* odd n */
        return (nn * nn + nn * 4 + 3) / 4;
    else          /* even n */
        return (nn * nn + nn * 4) / 4;
}
