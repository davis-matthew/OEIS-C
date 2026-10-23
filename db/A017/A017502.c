/*
 * A017502  a(n) = (11*n + 9)^6.
 * https://oeis.org/A017502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 9)^6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 146, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n + (seq_t)9;
    seq_t result = 1;
    for (unsigned i = 0; i < 6; ++i) {
        result *= base;
    }
    return result;
}
