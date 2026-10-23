/*
 * A017241  a(n) = (9*n + 6)^9.
 * https://oeis.org/A017241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 6)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 14, u128 n <= 2122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(9 * n + 6);
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i) {
        result *= base;
    }
    return result;
}
