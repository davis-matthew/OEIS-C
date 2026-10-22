/*
 * A004763  Numbers whose binary expansion does not begin 101.
 * https://oeis.org/A004763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 0, a(2) = 1, a(3) = 2, a(2^m+k+2) = 2^m + 2^(m-1) + k, m
 *            >= 1, 0 <= k < 2^m
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;
    
    // Find the largest power of 2 that is <= n - 2
    idx_t power = 1;
    while (power * 2 <= n - 2) {
        power *= 2;
    }
    
    // Compute k = n - (2^m + 2) where power = 2^m
    idx_t k = n - (power + 2);
    
    // Return 2^m + 2^(m-1) + k = power + power/2 + k
    return (seq_t)power + ((seq_t)power >> 1) + (seq_t)k;
}
