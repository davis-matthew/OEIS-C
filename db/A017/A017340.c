/*
 * A017340  a(n) = (10*n + 5)^12.
 * https://oeis.org/A017340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 3, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)10 * n + (seq_t)5;
    seq_t result = 1;
    for (int i = 0; i < 12; ++i) {
        result *= base;
    }
    return result;
}
