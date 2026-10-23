/*
 * A013804  a(n) = 16^(4*n + 1).
 * https://oeis.org/A013804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t e = 4 * n + 1;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) {
        result *= (seq_t)16;
    }
    return result;
}
