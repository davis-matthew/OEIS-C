/*
 * A000431  Expansion of 2*x^3/((1-2*x)^2*(1-4*x)).
 * https://oeis.org/A000431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n-3) - n*2^(n-2), n >= 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    
    seq_t sn = (seq_t)n;
    seq_t power2_2n_minus_3 = (seq_t)1 << (2*n - 3);
    seq_t power2_n_minus_2 = (seq_t)1 << (n - 2);
    
    return power2_2n_minus_3 - sn * power2_n_minus_2;
}
