/*
 * A089898  Product of (digits of n each incremented by 1).
 * https://oeis.org/A089898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = product of (digit+1) for each decimal digit of n; initial
 *            value a(0) = 1
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;               /* start with 1 */
    while (n > 0)                         /* iterate over the digits of n */
    {
        int d = n % 10;                   /* current least‑significant digit */
        prod *= (seq_t)(d + 1);           /* multiply by (digit+1) */
        n /= 10;                          /* drop that digit */
    }
    return prod;                          /* works also for n==0, yielding 1 */
}
