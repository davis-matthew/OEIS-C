/*
 * A005945  Number of n-step mappings with 4 inputs.
 * https://oeis.org/A005945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n-1)*(2*n-1)/2
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    
    if (n % 2 == 0) {
        return (n_s / 2) * (3 * n_s - 1) * (2 * n_s - 1);
    } else {
        return n_s * ((3 * n_s - 1) / 2) * (2 * n_s - 1);
    }
}
