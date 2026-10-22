/*
 * A004759  Binary expansion starts 111.
 * https://oeis.org/A004759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2^m + k) = 7 * 2^m + k, m >= 0, 0 <= k < 2^m
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Find the highest power of 2 <= n
    idx_t pow2 = 1;
    while (pow2 * 2 <= n) {
        pow2 *= 2;
    }
    
    idx_t k = n - pow2;
    return (seq_t)7 * (seq_t)pow2 + (seq_t)k;
}
