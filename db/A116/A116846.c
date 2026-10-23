/*
 * A116846  Number of permutations of length n which avoid the patterns 321,
 *          12354.
 * https://oeis.org/A116846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 9n^5 - 5n^4 - 405n^3 + 1804n^2 - 684n - 2160)/720
 *            (for n >= 3); a(0) = 1, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t ns = (seq_t)n;

    seq_t n2 = ns * ns;          /* n^2 */
    seq_t n3 = n2 * ns;          /* n^3 */
    seq_t n4 = n3 * ns;          /* n^4 */
    seq_t n5 = n4 * ns;          /* n^5 */
    seq_t n6 = n5 * ns;          /* n^6 */

    seq_t num = n6
              + (seq_t)9   * n5
              - (seq_t)5   * n4
              - (seq_t)405 * n3
              + (seq_t)1804 * n2
              - (seq_t)684 * ns
              - (seq_t)2160;

    return num / (seq_t)720;
}
