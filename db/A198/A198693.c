/*
 * A198693  a(n) = 3*4^n-1.
 * https://oeis.org/A198693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^n -1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 2 */
    seq_t val = 2;
    for (idx_t i = 0; i < n; ++i) {
        /* a(k+1) = 4*a(k) + 3 */
        val = val * 4 + 3;
    }
    return val;
}
