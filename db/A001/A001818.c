/*
 * A001818  Squares of double factorials: (1*3*5*...*(2n-1))^2 =
 *          ((2*n-1)!!)^2.
 * https://oeis.org/A001818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ((2*n-1)!!)^2 where (2*n-1)!! = 1*3*5*...*(2*n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; i++) {
        prod = prod * ((seq_t)i * 2 - 1);
    }
    return prod * prod;
}
