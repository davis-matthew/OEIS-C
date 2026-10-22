/*
 * A001814  Coefficient of H_2 when expressing x^{2n} in terms of Hermite
 *          polynomials H_m.
 * https://oeis.org/A001814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)! / (2*(n-1)!) = n * (n+1) * ... * (2*n) / 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    int divided_by_2 = 0;
    
    for (idx_t i = n; i <= 2 * n; i++) {
        if (!divided_by_2 && i % 2 == 0) {
            result *= (seq_t)(i / 2);
            divided_by_2 = 1;
        } else {
            result *= (seq_t)i;
        }
    }
    
    return result;
}
