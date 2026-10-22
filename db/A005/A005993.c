/*
 * A005993  Expansion of (1+x^2)/((1-x)^2*(1-x^2)^2).
 * https://oeis.org/A005993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(2n) = (n+1)(2n^2+4n+3)/3, a(2n+1) = (n+1)(n+2)(2n+3)/3
 * Range:     u8 n <= 12, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t k = (seq_t)(n / 2);
    
    if (n & 1) {  // a(2k+1)
        return (k + 1) * (k + 2) * (2 * k + 3) / 3;
    } else {  // a(2k)
        return (k + 1) * (2 * k * k + 4 * k + 3) / 3;
    }
}
