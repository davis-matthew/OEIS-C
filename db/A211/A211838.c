/*
 * A211838  Number of nonnegative integer arrays of length n+5 with new values
 *          0 upwards introduced in order, no three adjacent elements equal,
 *          and containing the value n+1.
 * https://oeis.org/A211838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/48*n^6 + 19/48*n^5 + 51/16*n^4 + 661/48*n^3 +
 *            835/24*n^2 + 317/6*n + 41. Equivalent integer form: a(n) = (n^6
 *            + 19*n^5 + 153*n^4 + 661*n^3 + 1670*n^2 + 2536*n + 1968) / 48.
 *            (The value is integer for all n≥1.)
 * Range:     u8 none, u16 n <= 3, u32 n <= 47, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s  = (seq_t)n;          /* n */
    seq_t n2 = s * s;             /* n^2 */
    seq_t n3 = n2 * s;            /* n^3 */
    seq_t n4 = n3 * s;            /* n^4 */
    seq_t n5 = n4 * s;            /* n^5 */
    seq_t n6 = n5 * s;            /* n^6 */

    /* Compute numerator of the integer formula */
    seq_t numer = n6
                + (seq_t)19   * n5
                + (seq_t)153  * n4
                + (seq_t)661  * n3
                + (seq_t)1670 * n2
                + (seq_t)2536 * s
                + (seq_t)1968;   /* constant term */

    return numer / (seq_t)48;     /* denominator */
}
