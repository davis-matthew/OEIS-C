/*
 * A063258  a(n) = binomial(n+5,4) - 1.
 * https://oeis.org/A063258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,4) - 1
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 443, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* start with binomial(n+5,0) = 1 */
    for (int k = 1; k <= 4; ++k)
    {
        /* multiply by the next factor (n+5 - (k-1)) and divide by k
           this maintains an integer at every step */
        result = result * ((seq_t)n + 5 - (k - 1));
        result /= k;
    }
    result -= 1;               /* subtract 1 as the definition requires */
    return result;
}
