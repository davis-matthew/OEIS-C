/*
 * A129194  a(n) = (n/2)^2*(3 - (-1)^n).
 * https://oeis.org/A129194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn, mult
 * Formula:   a(n) = n^2*(3-(-1)^n)/4 i.e., a(n) = n^2 when n is odd = n^2 / 2
 *            when n is even
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 960, u64 n <= 960,
 *            u128 n <= 960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;   /* n^2 in seq_t domain */
    if ((n & 1) == 0)                   /* even n  */
        return n_sq / 2;                /* divide by 2 */
    else                                /* odd n   */
        return n_sq;
}
