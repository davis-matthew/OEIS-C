/*
 * A003674  a(n) = 2^(n-1)*(2^n - (-1)^n).
 * https://oeis.org/A003674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2^n - (-1)^n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t pow2_2n_minus_1 = (seq_t)1 << (2 * n - 1);
    seq_t pow2_n_minus_1 = (seq_t)1 << (n - 1);
    
    return (n % 2 == 0) ? (pow2_2n_minus_1 - pow2_n_minus_1) : (pow2_2n_minus_1 + pow2_n_minus_1);
}
