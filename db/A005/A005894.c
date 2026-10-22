/*
 * A005894  Centered tetrahedral numbers.
 * https://oeis.org/A005894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n + 1)*(n^2 + n + 3)/3
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    seq_t two_n_plus_1 = 2 * n_s + 1;
    seq_t n_sq_plus_n_plus_3 = n_s * n_s + n_s + 3;
    
    // One of the factors is always divisible by 3
    if (two_n_plus_1 % 3 == 0) {
        return (two_n_plus_1 / 3) * n_sq_plus_n_plus_3;
    } else {
        return two_n_plus_1 * (n_sq_plus_n_plus_3 / 3);
    }
}
