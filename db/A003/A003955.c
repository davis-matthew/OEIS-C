/*
 * A003955  a(n) = (2*n + 4) * (1*3*5*...*(2*n+1))^2.
 * https://oeis.org/A003955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 4) * (1*3*5*...*(2*n+1))^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t odd_product = 1;
    for (idx_t i = 0; i <= n; i++) {
        odd_product *= (2 * (seq_t)i + 1);
    }
    seq_t squared = odd_product * odd_product;
    return (2 * (seq_t)n + 4) * squared;
}
