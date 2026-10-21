/*
 * A224142  Number of n X 4 0..1 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A224142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 4*n^3 + 14*n^2 + 23*n - 12) / 3 (for n > 1) a(1) =
 *            11
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* defined only for n >= 1 */
    if (n == 1)
        return (seq_t)11;  /* explicit cast for consistency */

    seq_t nn = (seq_t)n;          /*  n   */
    seq_t n2 = nn * nn;          /*  n^2 */
    seq_t n3 = n2 * nn;          /*  n^3 */
    seq_t n4 = n2 * n2;          /*  n^4 */

    /* compute (n^4 + 4*n^3 + 14*n^2 + 23*n - 12) / 3 */
    seq_t val = ( n4
                + ((seq_t)4 * n3)
                + ((seq_t)14 * n2)
                + ((seq_t)23 * nn)
                - (seq_t)12 ) / (seq_t)3;

    return val;
}
