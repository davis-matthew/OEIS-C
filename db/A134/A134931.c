/*
 * A134931  a(n) = (5*3^n-3)/2.
 * https://oeis.org/A134931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*3^n-3)/2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        val = 3 * (val + 1);
    }
    return val;
}
