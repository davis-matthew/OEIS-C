/*
 * A026054  dot product (n,n-1,...2,1).(3,4,...,n,1,2).
 * https://oeis.org/A026054
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 9*n - 10)/6
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the computation type first */
    seq_t nn = (seq_t)n;

    /* Compute the product n * (n^2 + 9*n - 10) using seq_t arithmetic. */
    seq_t prod = nn * (nn * nn + 9 * nn - 10);

    /* The result is always divisible by 6 for n >= 3, so the integer division is exact. */
    return prod / 6;
}
