/*
 * A071054  a(2n)=3n+1, a(2n+1)=2n+2.
 * https://oeis.org/A071054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n)=3n+1, a(2n+1)=2n+3
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* k is floor(n/2) */
    seq_t k = (seq_t)(n >> 1);
    if ((n & 1ULL) == 0) {
        return (seq_t)3 * k + (seq_t)1;
    } else {
        return (seq_t)2 * k + (seq_t)3;
    }
}
