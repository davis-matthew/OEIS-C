/*
 * A002474  Denominators of coefficients of odd powers of x of the expansion
 *          of Bessel function J_1(x).
 * https://oeis.org/A002474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n+1) * n! * (n+1)! = 2 * prod_{i=1}^{n} 4*i*(i+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 2;
    
    for (idx_t i = 1; i <= n; i++) {
        result *= 4 * (seq_t)i * ((seq_t)i + 1);
    }
    
    return result;
}
