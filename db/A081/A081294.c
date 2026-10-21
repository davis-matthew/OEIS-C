/*
 * A081294  Expansion of (1-2*x)/(1-4*x).
 * https://oeis.org/A081294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) , a(0)=1 , a(1)=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t val = (seq_t)2;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        val *= (seq_t)4;          /* recurrence a(i) = 4*a(i-1) */
    return val;
}
