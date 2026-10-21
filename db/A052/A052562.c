/*
 * A052562  a(n) = 5^n * n!.
 * https://oeis.org/A052562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * n! (recurrence a(0)=1, a(n)=5*n*a(n-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        result *= (seq_t)5 * (seq_t)i;   /* multiply by 5*n each step */
    }
    return result;
}
