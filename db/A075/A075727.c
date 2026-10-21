/*
 * A075727  a(n) = 2 Pi * n rounded off.
 * https://oeis.org/A075727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round( 2*π*(n-1) ) Approximate π by the convergent
 *            355/113 giving 2π ≈ 710/113. Rounding is achieved by (t +
 *            56)/113, where t = (n-1)*710
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not needed for offset=1, but safe */
    seq_t x = (seq_t)(n - 1);          /* n>=1 -> n-1>=0 */
    seq_t t = x * ((seq_t)710);        /* (n-1)*710 */
    seq_t r = (t + ((seq_t)56)) / ((seq_t)113); /* round nearest */
    return r;
}
