/*
 * A017609  a(n) = (12*n + 7)^5.
 * https://oeis.org/A017609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 7)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 593, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n) * 12 + 7;
    seq_t result = 1;
    for (int i = 0; i < 5; ++i) {
        result *= base;
    }
    return result;
}
