/*
 * A185787  Sum of first k numbers in column k of the natural number array
 *          A000027; by antidiagonals.
 * https://oeis.org/A185787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n^2-6*n+5)/6
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 977, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* safety, though the sequence starts at n=1 */
        return (seq_t)0;
    seq_t s = (seq_t)n;
    /* Compute 7*n^2 - 6*n + 5 */
    seq_t temp = 7 * s * s - 6 * s + (seq_t)5;
    /* Compute n * temp / 6 */
    seq_t num = s * temp;
    return num / (seq_t)6;
}
