/*
 * A033934  a(n) = (10^n + 1)^2.
 * https://oeis.org/A033934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^n + 1)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = 1;
    for (idx_t i = 0; i < n; ++i) {
        p *= 10;
    }
    seq_t s = p + 1;
    return s * s;
}
