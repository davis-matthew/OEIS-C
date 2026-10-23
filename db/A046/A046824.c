/*
 * A046824  Number of 1's in binary expansion of 5n+4.
 * https://oeis.org/A046824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 1's in the binary expansion of (5*n + 4)
 * Range:     u8 n <= 50, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5*n + 4 */
    seq_t m = (seq_t)n;
    m *= (seq_t)5;
    m += (seq_t)4;

    /* Count set bits */
    seq_t cnt = 0;
    while (m != (seq_t)0) {
        cnt++;                /* one set bit found */
        m &= m - (seq_t)1;    /* clear the lowest set bit */
    }
    return cnt;
}
