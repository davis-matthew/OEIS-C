/*
 * A016831  a(n) = (4n+2)^7.
 * https://oeis.org/A016831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)4 * (seq_t)n) + 2;
    seq_t result = 1;
    for (int i = 0; i < 7; ++i) {
        result *= base;
    }
    return result;
}
