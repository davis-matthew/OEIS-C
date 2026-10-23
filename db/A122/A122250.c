/*
 * A122250  Partial sums of A004128.
 * https://oeis.org/A122250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=0..n} Sum_{k=0..j} floor(j/3^k)
 * Range:     u8 n <= 18, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t f = (seq_t)j;            /* term for k = 0 : floor(j/1) = j */
        idx_t x = j;                   /* current divisor power of 3 */
        while (x > 0)
        {
            x /= 3;
            f += (seq_t)x;            /* add floor(j/3^k) for k >= 1 */
        }
        result += f;
    }
    return result;
}
