/*
 * A116834  Number of permutations of length n which avoid the patterns 1234,
 *          2431, 3421.
 * https://oeis.org/A116834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=6, for n>=4 a(n) = (35*n^4 - 390*n^3 +
 *            1729*n^2 - 3438*n + 2592) / 24
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    seq_t ns   = (seq_t)n;
    seq_t n2   = ns * ns;          /* n^2   */
    seq_t n3   = n2 * ns;          /* n^3   */
    seq_t n4   = n3 * ns;          /* n^4   */

    /* Positive part of the numerator */
    seq_t pos = (seq_t)35 * n4 + (seq_t)1729 * n2 + (seq_t)2592;
    /* Negative part of the numerator */
    seq_t neg = (seq_t)390 * n3 + (seq_t)3438 * ns;

    seq_t num = pos - neg;          /* guaranteed non‑negative for n >= 4 */
    return num / (seq_t)24;
}
