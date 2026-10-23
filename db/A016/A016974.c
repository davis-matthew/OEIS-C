/*
 * A016974  a(n) = (6*n + 5)^6.
 * https://oeis.org/A016974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n+5)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 270, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)n * (seq_t)6 + (seq_t)5;
    seq_t result = (seq_t)1;
    for (int i = 0; i < 6; ++i) {
        result *= base;
    }
    return result;
}
