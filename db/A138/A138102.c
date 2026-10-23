/*
 * A138102  The number 2*k^2 repeated 2*k^2 times, k=1 to 4.
 * https://oeis.org/A138102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full
 * Formula:   a(n) = 2*k^2 where k is the smallest integer with * n <=
 *            k(k+1)(2k+1)/3 (i.e. n lies in the block of length 2*k^2 )
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n < 1 the sequence is undefined; we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Find the block index k by cumulative summation. */
    idx_t k      = 0;   /* current k */
    idx_t sum    = 0;   /* cumulative length up to block k */

    while (sum < n) {
        ++k;                       /* next block */
        sum += 2 * k * k;          /* block length = 2*k^2 */
    }

    /* value for this block */
    return (seq_t)(2 * k * k);
}
