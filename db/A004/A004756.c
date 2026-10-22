/*
 * A004756  Binary expansion starts 100.
 * https://oeis.org/A004756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n + 3 * 2^floor(log_2(n))
 * Range:     u8 n <= 63, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t power = 1;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        power <<= 1;
    }
    
    return (seq_t)n + 3 * (seq_t)power;
}
