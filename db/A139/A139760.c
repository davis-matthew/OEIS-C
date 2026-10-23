/*
 * A139760  First quadrisection of A115451.
 * https://oeis.org/A139760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 7, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 1;
    for (idx_t i = 1; i <= n; ++i) {
        v = v * 16 - 7;
    }
    return v;
}
