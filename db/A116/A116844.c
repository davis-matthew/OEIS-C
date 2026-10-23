/*
 * A116844  Number of permutations of length n which avoid the patterns 231,
 *          12345.
 * https://oeis.org/A116844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 9n^5 + 43n^4 - 99n^3 + 172n^2 - 108n + 144)/144
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;

    seq_t n2 = n_t * n_t;           /* n^2 */
    seq_t n3 = n2 * n_t;            /* n^3 */
    seq_t n4 = n3 * n_t;            /* n^4 */
    seq_t n5 = n4 * n_t;            /* n^5 */
    seq_t n6 = n5 * n_t;            /* n^6 */

    seq_t num = n6
              - (seq_t)9  * n5
              + (seq_t)43 * n4
              - (seq_t)99 * n3
              + (seq_t)172 * n2
              - (seq_t)108 * n_t
              + (seq_t)144;

    return num / (seq_t)144;
}
