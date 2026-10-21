/*
 * A232806  a(n) = a(n-1)^2/2 + 5/2, a(0) = 1.
 * https://oeis.org/A232806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2/2 + 5/2, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1; /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        res = (res * res + 5) / 2;
    }
    return res;
}
