/*
 * A000389  Binomial coefficients C(n,5).
 * https://oeis.org/A000389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(n,5) = n*(n-1)*(n-2)*(n-3)*(n-4)/120
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 5) return 0;
    
    seq_t result = 1;
    for (idx_t i = 1; i <= 5; i++) {
        result *= (seq_t)(n - i + 1);
        result /= (seq_t)i;
    }
    
    return result;
}
