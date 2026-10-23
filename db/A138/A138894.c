/*
 * A138894  Expansion of (1+x)/(1-10*x+9*x^2).
 * https://oeis.org/A138894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9 * a(n-1) + 2, with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;          /* a(0) */
    for (idx_t i = (idx_t)0; i < n; ++i)
    {
        res = (res * (seq_t)9) + (seq_t)2;
    }
    return res;
}
