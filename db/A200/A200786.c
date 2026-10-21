/*
 * A200786  Number of 0..n arrays x(0..3) of 4 elements without any two
 *          consecutive increases.
 * https://oeis.org/A200786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (17*n^4 + 86*n^3 + 151*n^2 + 106*n + 24)/24
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 140, u64 n <= 140,
 *            u128 n <= 140, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200786: number of 0..n arrays x[0..3] of four elements
   with no two consecutive increases.                                     */
seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined, but we return 0 safely. */
    if (n == 0)
        return 0;

    /* Use 128-bit arithmetic to avoid overflow on the intermediate
       values before the final division.   */
    unsigned __int128 nn  = n;
    unsigned __int128 n2  = nn * nn;        /* n^2   */
    unsigned __int128 n3  = n2 * nn;        /* n^3   */
    unsigned __int128 n4  = n3 * nn;        /* n^4   */

    /* Compute the numerator of the rational expression. */
    unsigned __int128 num =
        (unsigned __int128)17 * n4 +
        (unsigned __int128)86 * n3 +
        (unsigned __int128)151 * n2 +
        (unsigned __int128)106 * nn +
        (unsigned __int128)24;

    num /= 24;              /* exact division, numerator always divisible by 24 */

    return (seq_t)num;
}
