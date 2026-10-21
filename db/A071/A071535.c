/*
 * A071535  (-1)^(n+1) * Determinant of n X n matrix of form [1^2 2^2 3^2 4^2
 *          5^2 / 2^2 1^2 2^2 3^2 4^2 / 3^2 2^2 1^2 2^2 3^2 / 4^2 3^2 2^2 1^2
 *          2^2 / 5^2 4^2 3^2 2^2 1^2].
 * https://oeis.org/A071535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(-5+2*n)*(6+8*n+5*n^2+4*n^3+n^4)/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the polynomial P(n) = 6 + 8n + 5n^2 + 4n^3 + n^4 */
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t nn4 = nn3 * nn;

    seq_t p = 6;
    p += (seq_t)8 * nn;       /* 8n */
    p += (seq_t)5 * nn2;      /* 5n^2 */
    p += (seq_t)4 * nn3;      /* 4n^3 */
    p += nn4;                 /* n^4 */

    /* Shift amount is 2*n - 5 */
    int64_t shift = (int64_t)n * 2 - 5;
    if (shift >= 0) {
        p <<= shift;          /* multiply by 2^(shift) */
    } else {
        p >>= -shift;         /* divide by 2^(-shift) */
    }

    /* Divide by 3.  The division is exact for all n >= 1. */
    p /= 3;

    return p;
}
