/*
 * A027875  a(n) = Product_{i=1..n} (7^i - 1).
 * https://oeis.org/A027875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (7^i - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t power  = 1;               /* will hold 7^i during the loop */
    for (idx_t i = 1; i <= n; ++i)
    {
        power *= 7;                 /* compute 7^i */
        seq_t term = power - 1;     /* (7^i - 1) */
        result *= term;             /* multiply into running product */
    }
    return result;
}
