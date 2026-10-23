/*
 * A180291  Number of arrangements of n indistinguishable balls in n boxes
 *          with the maximum number of balls in any box equal to n-1.
 * https://oeis.org/A180291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1) for n > 2; a(2) = 1
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* out of range (not used) */
    if (n == 2)
        return (seq_t)1;          /* a(2) = 1 */
    return (seq_t)n * ((seq_t)n - (seq_t)1);
}
