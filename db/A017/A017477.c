/*
 * A017477  a(n) = (11*n + 7)^5.
 * https://oeis.org/A017477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 7)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 647, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 11 + 7;
    seq_t result = 1;
    for (idx_t i = 0; i < 5; ++i) {
        result = result * base;
    }
    return result;
}
