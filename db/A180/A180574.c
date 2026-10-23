/*
 * A180574  Wiener index of the n-sunlet graph.
 * https://oeis.org/A180574
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+4*n-2)/2 if n even; a(n) = n*(n^2+4*n-3)/2 if n
 *            odd
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result;
    if ((n & 1ULL) == 0) {                       /* even n */
        seq_t tmp = nn * nn + 4 * nn - 2;
        result = nn * tmp / 2;
    } else {                                     /* odd n */
        seq_t tmp = nn * nn + 4 * nn - 3;
        result = nn * tmp / 2;
    }
    return result;
}
