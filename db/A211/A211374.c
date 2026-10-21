/*
 * A211374  Product of all the parts in the partitions of n into exactly 2
 *          parts.
 * https://oeis.org/A211374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..floor(n/2)} i*(n-i)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 2;           /* floor(n/2) */
    seq_t prod = (seq_t)1;
    for (idx_t j = 1; j <= m; ++j) {
        prod *= (seq_t)j;
        prod *= (seq_t)(n - j);
    }
    return prod;
}
