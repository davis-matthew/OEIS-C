/*
 * A016856  a(n) = (5*n)^8.
 * https://oeis.org/A016856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 51, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)5 * (seq_t)n;
    seq_t result = (seq_t)1;
    for (int i = 0; i < 8; ++i) {
        result *= base;
    }
    return result;
}
