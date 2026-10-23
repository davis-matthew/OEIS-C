/*
 * A047866  a(n) = ceiling(n*(n+1)*(n+2)/8).
 * https://oeis.org/A047866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(n+1)*(n+2)/8)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1947, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t early to avoid overflow on larger seq_t types */
    seq_t x = (seq_t)n;
    /* Compute numerator = n*(n+1)*(n+2) */
    seq_t prod = x * (x + 1);
    prod *= (x + 2);
    /* Add 7 to perform ceil division by 8: ceil(k/8) = (k+7)/8 */
    prod += 7;
    return prod / 8;
}
