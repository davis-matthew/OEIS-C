/*
 * A017045  a(n) = (7*n + 5)^5.
 * https://oeis.org/A017045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 11, u64 n <= 1018, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n * (seq_t)7) + (seq_t)5;
    seq_t result = base;
    for (int i = 1; i < 5; ++i) {
        result *= base;
    }
    return result;
}
