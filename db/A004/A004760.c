/*
 * A004760  List of numbers whose binary expansion does not begin 10.
 * https://oeis.org/A004760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n-2) + 2^ceiling(log_2(n-1)) for n >= 2; a(1) = 0
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    idx_t m = n - 1;
    
    // Find ceiling(log_2(m))
    idx_t msb_pos = 63 - __builtin_clzll(m);
    idx_t ceil_log2_m;
    if (m == (1ULL << msb_pos)) {
        ceil_log2_m = msb_pos;
    } else {
        ceil_log2_m = msb_pos + 1;
    }
    
    // Compute 2^ceiling(log_2(m))
    seq_t pow2 = (seq_t)1 << ceil_log2_m;
    
    return (seq_t)(n - 2) + pow2;
}
