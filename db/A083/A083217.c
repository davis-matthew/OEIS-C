/*
 * A083217  a(n) = (2*5^n + (-1)^n)/3.
 * https://oeis.org/A083217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*5^n + (-1)^n)/3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow5 *= (seq_t)5;         /* pow5 = 5^n */

    seq_t numerator = pow5 * (seq_t)2;
    if (n & 1)                     /* n is odd  -> (-1)^n = -1 */
        numerator -= (seq_t)1;     /* numerator = 2*5^n - 1 */
    else                           /* n is even -> (-1)^n =  1 */
        numerator += (seq_t)1;     /* numerator = 2*5^n + 1 */

    return numerator / (seq_t)3;   /* integer division gives a(n) */
}
