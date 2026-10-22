/*
 * A006963  Number of planar embedded labeled trees with n nodes:
 *          (2*n-3)!/(n-1)! for n >= 2, a(1) = 1.
 * https://oeis.org/A006963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n-3)! / (n-1)! for n >= 2, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    
    seq_t result = (seq_t)1;
    for (idx_t i = n; i <= 2*n - 3; i++) {
        result *= (seq_t)i;
    }
    return result;
}
