/*
 * A052787  Product of 5 consecutive integers.
 * https://oeis.org/A052787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4) (product of 5 consecutive
 *            integers)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < 5; ++i)
    {
        prod *= (seq_t)(n - i);  /* unsigned arithmetic ensures a(0)…a(4) = 0 */
    }
    return prod;
}
