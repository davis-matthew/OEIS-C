/*
 * A218289  Denominator of the sixth increasing diagonal of the autosequence
 *          of the second kind from (-1)^n/(n+1).
 * https://oeis.org/A218289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)/s(n) where s(n) repeats the cycle
 *            1,2,5,10,1,2,1,10,5,2
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t s[10] = {1, 2, 5, 10, 1, 2, 1, 10, 5, 2};
    seq_t nn = (seq_t)n;
    seq_t denom = s[n % 10];
    seq_t num   = (nn + 1) * (nn + 2) * (nn + 3);   /* exact division by denom */
    return num / denom;
}
