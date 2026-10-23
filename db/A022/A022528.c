/*
 * A022528  Nexus numbers (n+1)^12-n^12.
 * https://oeis.org/A022528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^12 - n^12 (expanded)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 44, u128 n <= 2539,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients of 12*n^11 + 66*n^10 + ... + 12*n + 1   */
    static const seq_t coeffs[12] = {12, 66, 220, 495, 792, 924,
                                     792, 495, 220, 66, 12, 1};
    seq_t nn = (seq_t)n;
    seq_t res = coeffs[0];
    for (int i = 1; i < 12; ++i)
        res = res * nn + coeffs[i];
    return res;
}
