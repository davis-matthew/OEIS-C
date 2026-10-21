/*
 * A211380  Number of pairs of intersecting diagonals in the interior and
 *          exterior of a regular n-gon.
 * https://oeis.org/A211380
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n even: a(n) = n * (n^3 - 11*n^2 + 43*n - 58) / 8 ; if n odd
 *            : a(n) = n * (n-3) * (n^2 - 8*n + 19) / 8
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;           /* values before offset are irrelevant */

    seq_t nn = (seq_t)n;
    seq_t res;

    if ((n & 1) == 0) {                   /* even n */
        seq_t n2 = nn * nn;              /* n^2 */
        seq_t n3 = n2 * nn;              /* n^3 */
        seq_t term = n3
                     - (seq_t)11 * n2
                     + (seq_t)43 * nn
                     - (seq_t)58;
        res = nn * term / (seq_t)8;
    } else {                              /* odd n */
        seq_t n2 = nn * nn;              /* n^2 */
        seq_t term = n2
                     - (seq_t)8 * nn
                     + (seq_t)19;
        seq_t factor = nn * (nn - (seq_t)3);
        res = factor * term / (seq_t)8;
    }

    return res;
}
