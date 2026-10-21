/*
 * A051262  10-factorial numbers.
 * https://oeis.org/A051262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * 10^n (equivalently a(n) = ∏_{k=1..n} 10*k, a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= n; ++i)
    {
        result *= (seq_t)i;   /* multiply by k */
        result *= 10;        /* multiply by 10 */
    }
    return result;
}
