/*
 * A123680  a(n) = Sum_{k=0..n} C(n+k-1,k)*k!.
 * https://oeis.org/A123680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n+k-1,k) * k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 1;          /* k = 0 term: (n)_0 = 1          */
    seq_t prod = 1;          /* running product of (n)_k        */
    for (idx_t k = 1; k <= n; ++k)
    {
        prod *= (seq_t)(n + k - 1);  /* multiply by (n + k - 1)   */
        sum += prod;                /* add the new term          */
    }
    return sum;
}
