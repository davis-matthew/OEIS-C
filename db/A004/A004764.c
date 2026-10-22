/*
 * A004764  Numbers whose binary expansion does not begin 110.
 * https://oeis.org/A004764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n - 1 for 1 <= n <= 6; a(2^(m+2) + 2^m + k + 2) = 2^(m+2)
 *            + 2^(m+1) + 2^m + k, m >= 0, 0 <= k < 2^(m+2) + 2^m
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 6) {
        return (seq_t)(n - 1);
    }
    
    // Find which block n belongs to
    idx_t cumulative = 6;
    idx_t m = 0;
    
    while (1) {
        idx_t power_m2 = 1ULL << (m + 2);
        idx_t power_m = 1ULL << m;
        idx_t block_size = power_m2 + power_m;
        
        if (n <= cumulative + block_size) {
            break;
        }
        
        cumulative += block_size;
        m++;
    }
    
    // Compute k
    idx_t power_m2 = 1ULL << (m + 2);
    idx_t power_m = 1ULL << m;
    idx_t k = n - 2 - power_m2 - power_m;
    
    // Compute result: 2^(m+2) + 2^(m+1) + 2^m + k
    seq_t result = (seq_t)power_m2 + (seq_t)(1ULL << (m + 1)) + (seq_t)power_m + (seq_t)k;
    return result;
}
