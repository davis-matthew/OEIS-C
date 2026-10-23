/*
 * A128625  Expansion of (1+3*x)/(1-5*x).
 * https://oeis.org/A128625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = expansion of (1+3*x)/(1-5*x) => a(0)=1, a(1)=8, and for
 *            n>=2 a(n)=5*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)8;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val *= (seq_t)5;           /* recurrence step */
    }
    return val;
}
