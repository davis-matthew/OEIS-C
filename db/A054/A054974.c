/*
 * A054974  Number of nonnegative integer 2 X 2 matrices with no zero rows or
 *          columns and with sum of elements equal to n, up to row and column
 *          permutation.
 * https://oeis.org/A054974
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For even n: a(n) = (2*n^3 + 12*n^2 - 8*n) / 48 For odd n: a(n) =
 *            (2*n^3 + 12*n^2 - 26*n + 12) / 48 (This is equivalent to the
 *            explicit closed form given in OEIS.)
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The offset is n >= 2.  For n < 2 the behavior is unspecified,
       but returning 0 is harmless and keeps the function well-defined. */
    if (n < 2) return 0;

    seq_t nn = (seq_t)n;            /* cast to seq_t for all arithmetic */
    seq_t nn_sq = nn * nn;          /* n^2 */
    seq_t nn_cube = nn_sq * nn;     /* n^3 */
    seq_t term;

    if ((n & 1) == 0) {            /* n even */
        term = 2 * nn_cube;        /* 2*n^3 */
        term += 12 * nn_sq;        /* +12*n^2 */
        term -= 8 * nn;            /* -8*n  */
    } else {                       /* n odd */
        term = 2 * nn_cube;        /* 2*n^3 */
        term += 12 * nn_sq;        /* +12*n^2 */
        term -= 26 * nn;           /* -26*n */
        term += 12;                /* +12   */
    }

    return term / 48;
}
