/*
 * A000722  Number of invertible Boolean functions of n variables: a(n) =
 *          (2^n)!.
 * https://oeis.org/A000722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = 1;
    for (idx_t i = 0; i < n; i++) {
        m *= 2;
    }
    
    seq_t result = 1;
    for (seq_t i = 2; i <= m; i++) {
        result *= i;
    }
    
    return result;
}
