/*
 * A008743  Molien series for 3-dimensional group [3,4]+ = 432.
 * https://oeis.org/A008743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2 + n*(3+9*(-1)^n) + 14*(-1)^n + 35)/48)
 * Range:     u8 n <= 9, u16 n <= 434, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t sign = ((n & 1) ? (seq_t)-1 : (seq_t)1);
    seq_t num = nn * nn
                + nn * ((seq_t)3 + (seq_t)9 * sign)
                + (seq_t)14 * sign
                + (seq_t)35;
    return num / (seq_t)48;
}
