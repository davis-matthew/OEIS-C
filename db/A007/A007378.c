/*
 * A007378  a(n), for n >= 2, is smallest positive integer which is consistent
 *          with sequence being monotonically increasing and satisfying
 *          a(a(n)) = 2n.
 * https://oeis.org/A007378
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(2^i + j) = 3*2^(i-1) + j, 0<=j<2^(i-1); a(3*2^(i-1) + j) =
 *            2^(i+1) + 2*j, 0<=j<2^(i-1)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t power = 1;
    while ((power << 1) <= n) {
        power <<= 1;
    }
    
    idx_t half_power = power >> 1;
    idx_t threshold = 3 * half_power;
    idx_t double_power = power << 1;
    
    if (n < threshold) {
        idx_t j = n - power;
        return (seq_t)(3 * half_power + j);
    } else {
        idx_t j = n - threshold;
        return (seq_t)(double_power + 2 * j);
    }
}
