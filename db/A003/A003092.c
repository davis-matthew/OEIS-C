/*
 * A003092  Number of labeled plane 2-trees with n nodes.
 * https://oeis.org/A003092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)^2*(5*n-10)!/(4*n-6)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = (seq_t)n;
    result *= ((seq_t)(n - 1) * (seq_t)(n - 1));
    
    if (n >= 4) {
        for (idx_t i = 4 * n - 5; i <= 5 * n - 10; i++) {
            result *= (seq_t)i;
        }
    } else {
        for (idx_t i = 5 * n - 9; i <= 4 * n - 6; i++) {
            result /= (seq_t)i;
        }
    }
    
    return result;
}
