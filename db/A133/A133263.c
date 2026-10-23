/*
 * A133263  Binomial transform of (1, 2, 0, 1, -1, 1, -1, 1, ...).
 * https://oeis.org/A133263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n + 4)/2 for n>0, a(0)=1
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;
    return t + (seq_t)2;
}
