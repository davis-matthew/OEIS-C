/*
 * A025765  Expansion of 1/((1-x)(1-x^2)(1-x^9)).
 * https://oeis.org/A025765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor((n^2 + 12*n + 9)/36)
 * Range:     u8 n <= 10, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t temp = nn * nn + (seq_t)12 * nn + (seq_t)9;
    return temp / (seq_t)36 + (seq_t)1;
}
