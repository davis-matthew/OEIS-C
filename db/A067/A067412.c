/*
 * A067412  Fourth column of triangle A067410.
 * https://oeis.org/A067412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 5 * 8^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)5;           /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        res = res * (seq_t)8;       /* multiply by 8 each step */
    }
    return res;
}
