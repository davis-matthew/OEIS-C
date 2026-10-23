/*
 * A116722  Number of permutations of length n which avoid the patterns 312,
 *          1324, 3421; or avoid the patterns 312, 1324, 2341, etc.
 * https://oeis.org/A116722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 6*n^3 + 47*n^2 - 114*n + 120)/24 for n >= 2; a(1)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 279, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A116722 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t numerator = n4
        - (seq_t)6 * n3
        + (seq_t)47 * n2
        - (seq_t)114 * nn
        + (seq_t)120;
    return numerator / (seq_t)24;
}
