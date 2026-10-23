/*
 * A024394  a(n) is the sum of squares of the first n positive integers
 *          congruent to 2 mod 3.
 * https://oeis.org/A024394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^3 + 3*n^2 - n)/2
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 991, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;       /* cast once to avoid intermediate overflow     */
    seq_t N2 = N * N;          /* n^2                                            */
    seq_t N3 = N2 * N;         /* n^3                                            */
    seq_t val = 6 * N3 + 3 * N2 - N;
    val /= 2;
    return val;
}
