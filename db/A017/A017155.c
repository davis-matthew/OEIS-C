/*
 * A017155  a(n) = (8*n + 7)^7.
 * https://oeis.org/A017155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^7
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 69, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)8 * n + (seq_t)7;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < (idx_t)7; ++i) {
        result *= base;
    }
    return result;
}
