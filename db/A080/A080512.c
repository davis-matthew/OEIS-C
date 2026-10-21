/*
 * A080512  a(n) = n if n is odd, a(n) = 3*n/2 if n is even.
 * https://oeis.org/A080512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n if n is odd, a(n) = 3*n/2 if n is even
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = (seq_t)n;
    if ((n & 1) == 0) {
        res = ((seq_t)n * (seq_t)3) / (seq_t)2;
    }
    return res;
}
