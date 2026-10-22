/*
 * A004757  Binary expansion starts 101.
 * https://oeis.org/A004757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2^m + k) = 5*2^m + k, where m = floor(log_2(n)), k = n - 2^m
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t power_of_2 = 1;
    
    // Find the highest power of 2 that is <= n_val
    while (power_of_2 <= n_val / 2) {
        power_of_2 *= 2;
    }
    
    seq_t k = n_val - power_of_2;
    return 5 * power_of_2 + k;
}
