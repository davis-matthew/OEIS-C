/*
 * A052560  a(n) = 3*n!.
 * https://oeis.org/A052560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;          /* fact = 1 for 0! */
    for (idx_t i = 1; i <= n; ++i)
    {
        fact = fact * (seq_t)i;   /* multiply by i for each new factorial value */
    }
    return fact * 3;          /* a(n) = 3 * n! */
}
