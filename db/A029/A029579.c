/*
 * A029579  a(2*n) = n+1, a(2*n-1) = 2*n-1.
 * https://oeis.org/A029579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = n + 1, a(2*n - 1) = 2*n - 1
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) /* odd index */
    {
        return (seq_t)n;           /* a(n) = n */
    }
    else
    {
        return (seq_t)(n / 2 + 1); /* a(n) = n/2 + 1 */
    }
}
