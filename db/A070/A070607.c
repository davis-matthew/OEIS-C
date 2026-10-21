/*
 * A070607  a(n) = n^5 mod 23.
 * https://oeis.org/A070607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 23
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 23 first, then compute the fifth power
     * using small intermediate values that fit in idx_t. */
    idx_t m = n % 23;
    idx_t acc = m;          /* n^1 mod 23 */
    /* Multiply four more times to reach the fifth power. */
    for (int i = 1; i < 5; ++i)
        acc = (acc * m) % 23;
    return (seq_t)acc;
}
