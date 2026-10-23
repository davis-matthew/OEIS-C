/*
 * A116728  Number of permutations of length n which avoid the patterns 321,
 *          1243, 2134.
 * https://oeis.org/A116728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n=1, a(n) = 2 for n=2, otherwise a(n) = 7*n - 16
 *            (OEIS A116728)
 * Range:     u8 n <= 38, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)n;          /* a(1)=1, a(2)=2 */
    return (seq_t)7 * (seq_t)n - (seq_t)16;   /* a(n)=7*n-16 for n>=3 */
}
