/*
 * A009120  a(n) = (4*n)!/(2*n)!.
 * https://oeis.org/A009120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)!/(2*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    idx_t base = 2 * n;
    for (idx_t i = 1; i <= 2 * n; ++i) {
        res *= (seq_t)(base + i);
    }
    return res;
}
