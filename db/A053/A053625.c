/*
 * A053625  Product of 6 consecutive integers.
 * https://oeis.org/A053625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5) (product of 6 consecutive
 *            integers)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 6 the product contains a factor 0, so a(n) = 0 */
    if (n < 6)
        return (seq_t)0;

    /* Compute the product using seq_t arithmetic; intermediate values are ≤ the final result,
       so no overflow occurs for values that fit in seq_t. */
    seq_t res = 1;
    for (idx_t i = 0; i < 6; ++i)
        res *= (seq_t)(n - i);

    return res;
}
