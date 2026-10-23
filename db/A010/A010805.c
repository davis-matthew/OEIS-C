/*
 * A010805  17th powers: a(n) = n^17.
 * https://oeis.org/A010805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^17
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 13, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t r = 1;
    for (int i = 0; i < 17; ++i) r *= x;
    return r;
}
