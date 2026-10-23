/*
 * A016769  a(n) = (3*n)^5.
 * https://oeis.org/A016769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 28, u64 n <= 2377,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t first */
    seq_t m = (seq_t)3 * n;
    seq_t r = m;
    /* multiply m by itself 4 more times to get (3n)^5 */
    for (int i = 1; i < 5; ++i) {
        r *= m;
    }
    return r;
}
