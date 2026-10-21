/*
 * A056699  First differences are 2,1,-2,3 (repeated).
 * https://oeis.org/A056699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + periodic[0,1,1,-2] (offset = 1)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1; compute n mod 4 */
    idx_t m = n & 3u;          /* n % 4 */
    switch (m)
    {
        case 1u:               /* a(n) = n                               */
            return (seq_t)n;
        case 2u:               /* a(n) = n + 1                           */
        case 3u:
            return (seq_t)n + 1;
        case 0u:               /* a(n) = n – 2                           */
            return (seq_t)n - 2;
        default:               /* unreachable, but keeps compiler quiet   */
            return (seq_t)0;
    }
}
