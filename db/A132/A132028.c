/*
 * A132028  Product{0<=k<=floor(log_4(n)), floor(n/4^k)}, n>=1.
 * https://oeis.org/A132028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0}^{⌊log_4(n)⌋} ⌊n/4^k⌋
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    /* loop over floor(n/4^k) for k = 0,1,2,... until the value becomes 0 */
    for (seq_t x = n; x > 0; x /= 4)
    {
        prod *= x;
    }
    return prod;
}
