/*
 * A017301  a(n) = (10*n + 2)^9.
 * https://oeis.org/A017301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 13, u128 n <= 1911,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)10 + (seq_t)2;
    seq_t result = 1;
    for (idx_t i = 0; i < (idx_t)9; ++i) {
        result *= base;
    }
    return result;
}
