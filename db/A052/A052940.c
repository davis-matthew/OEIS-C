/*
 * A052940  a(0) = 1; a(n) = 3*2^n - 1, for n > 0.
 * https://oeis.org/A052940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n) = 3*2^n - 1 for n > 0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    seq_t pow2 = (seq_t)1 << n;          /* compute 2^n   */
    return pow2 * (seq_t)3 - (seq_t)1;   /* 3*2^n - 1     */
}
