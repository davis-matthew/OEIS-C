/*
 * A192085  Number of ones in the binary expansion of n^3.
 * https://oeis.org/A192085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of ones in the binary expansion of n^3
 *            (popcount(n^3))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1904, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): returns popcount(n^3). */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;           /* n as the sequence type */
    x = x * x * x;                /* compute n^3 */

    seq_t cnt = 0;                /* popcount accumulator */
    while (x)                     /* while x != 0 */
    {
        cnt += x & 1;             /* add least significant bit */
        x >>= 1;                  /* shift right by 1 */
    }

    return cnt;                   /* number of ones */
}
