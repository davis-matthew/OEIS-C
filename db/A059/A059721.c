/*
 * A059721  Mean of first six positive powers of n, i.e., (n + n^2 + n^3 + n^4
 *          + n^5 + n^6)/6.
 * https://oeis.org/A059721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + n^2 + n^3 + n^4 + n^5 + n^6)/6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t p   = nn;      /* n^1 */
    seq_t sum = p;       /* sum = n */
    for (idx_t i = 2; i <= 6; ++i) {
        p = p * nn;      /* advance to next power */
        sum = sum + p;
    }
    return sum / 6;      /* integer division; exact for all n >= 1 */
}
