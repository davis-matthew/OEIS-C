/*
 * A011895  a(n) = floor(n*(n-1)*(n-2)/13).
 * https://oeis.org/A011895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/13)
 * Range:     u8 n <= 15, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the product is zero. */
    if (n < 3)
        return 0;

    /* Compute the product as seq_t and divide by 13. */
    return (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2) / 13;
}
