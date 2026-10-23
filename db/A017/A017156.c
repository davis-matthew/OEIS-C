/*
 * A017156  a(n) = (8*n + 7)^8.
 * https://oeis.org/A017156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 31, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n * (seq_t)8) + (seq_t)7;
    seq_t result = (seq_t)1;
    for (int i = 0; i < 8; ++i) {
        result *= base;
    }
    return result;
}
