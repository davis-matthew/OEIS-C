/*
 * A097138  Convolution of 4^n and floor(n/2).
 * https://oeis.org/A097138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} floor((n - k) / 2) * 4^k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow4   = 1;          /* 4^0 */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t coef = (seq_t)((n - k) >> 1);   /* floor((n-k)/2) */
        result += coef * pow4;
        pow4 *= 4;                          /* advance to next power of 4 */
    }
    return result;
}
