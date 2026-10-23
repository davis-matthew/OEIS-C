/*
 * A017267  a(n) = (9*n + 8)^11.
 * https://oeis.org/A017267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^11
 * Range:     u8 none, u16 none, u32 none, u64 n <= 5, u128 n <= 352,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n * (seq_t)9) + (seq_t)8;
    seq_t result = (seq_t)1;
    for (int i = 0; i < 11; ++i) {
        result = result * base;
    }
    return result;
}
