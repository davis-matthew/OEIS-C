/*
 * A204556  Left edge of the triangle A045975.
 * https://oeis.org/A204556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)/2 for even n; * a(n) = (n^3 - 2*n^2 + 3*n)/2
 *            for odd n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1643, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;              /* convert index to seq_t */
    seq_t n_sq = nn * nn;             /* n^2   */
    seq_t n_cu = n_sq * nn;           /* n^3   */

    if ((n & 1U) == 0) {              /* even n */
        return (n_cu - n_sq) / 2;
    } else {                          /* odd n */
        seq_t term2 = ((seq_t)2) * n_sq;
        seq_t term3 = ((seq_t)3) * nn;
        return (n_cu - term2 + term3) / 2;
    }
}
