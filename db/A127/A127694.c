/*
 * A127694  Absolute value of coefficient of x^3 in polynomial whose zeros are
 *          5 consecutive integers starting with the n-th integer.
 * https://oeis.org/A127694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+3)*(2*n^2+12*n+15)
 * Range:     u8 none, u16 n <= 15, u32 n <= 751, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;
    seq_t N = (seq_t)n;
    /* Compute 2*n^2 + 12*n + 15 */
    seq_t poly = (seq_t)2 * N * N + (seq_t)12 * N + (seq_t)15;
    /* Compute the final result */
    seq_t result = (seq_t)5 * (N + (seq_t)3) * poly;
    return result;
}
