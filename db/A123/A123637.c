/*
 * A123637  a(n) = 1 + 1*n + 1*n*2 + 1*n*2*(n-1) + 1*n*2*(n-1)*3 +
 *          1*n*2*(n-1)*3*(n-2) + ... + n!*(n+1)!.
 * https://oeis.org/A123637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 1*n + 1*n*2 + 1*n*2*(n-1) + ... + n!*(n+1)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 1;
    seq_t prod = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        prod *= (seq_t)(n - i);   /* multiply by decreasing factor n-i */
        sum += prod;              /* term after this multiplication */
        prod *= (seq_t)(i + 2);   /* multiply by increasing factor i+2 */
        sum += prod;              /* term after this multiplication */
    }
    return sum;
}
