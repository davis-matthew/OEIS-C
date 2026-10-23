/*
 * A194767  Denominator of the fourth increasing diagonal of the autosequence
 *          of second kind from (-1)^n / (n+1).
 * https://oeis.org/A194767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(3*n) = (3*n+1)*(3*n+2) a(3*n+1) = (n+1)*(3*n+2) a(3*n+2) =
 *            3*(n+1)*(3*n+4)
 * Range:     u8 n <= 14, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    seq_t res;
    if (r == 0) {
        idx_t m = n / 3;
        res = (seq_t)(3 * m + 1) * (seq_t)(3 * m + 2);
    } else if (r == 1) {
        idx_t m = n / 3;
        res = (seq_t)(m + 1) * (seq_t)(3 * m + 2);
    } else {
        idx_t m = n / 3;
        res = (seq_t)3 * (seq_t)(m + 1) * (seq_t)(3 * m + 4);
    }
    return res;
}
