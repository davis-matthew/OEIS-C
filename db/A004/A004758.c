/*
 * A004758  Binary expansion starts 110.
 * https://oeis.org/A004758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2^m+k) = 6*2^m + k, m >= 0, 0 <= k < 2^m
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    int m = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        m++;
    }
    
    seq_t pow2_m = (seq_t)1 << m;
    seq_t k = (seq_t)n - pow2_m;
    
    return 6 * pow2_m + k;
}
