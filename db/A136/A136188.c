/*
 * A136188  Digital roots of the Fermat numbers in A000215(n).
 * https://oeis.org/A136188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(0)=3; a(n)=5 if n>0 and n odd; a(n)=8 if n>0 and n even
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)3;
    }
    return (seq_t)((n & 1) ? 5 : 8);
}
