/*
 * A123684  Alternate A016777(n) with A000027(n).
 * https://oeis.org/A123684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%2==0) ? n/2 : 3*(n/2)+1
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=1, a(3)=4, a(4)=2, a(5)=7, a(6)=3, … */

seq_t a(idx_t n) {
    /* k = floor(n/2) */
    seq_t k = (seq_t)(n >> 1);
    return (n & 1) ? (k * 3 + 1) : k;
}
