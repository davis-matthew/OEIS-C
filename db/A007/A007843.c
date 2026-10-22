/*
 * A007843  Least positive integer k for which 2^n divides k!.
 * https://oeis.org/A007843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = least positive integer k for which 2^n divides k! Using
 *            the recursive formula: a(0) = 1, and for n > 0, if n = 2^i + r
 *            with 0 <= r < 2^i, then a(n) = 2^i + a(r+1) if r < 2^i - 1, and
 *            a(n) = 2^(i+1) if r = 2^i - 1
 * Range:     u8 n <= 247, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    
    // Find power = 2^i where 2^i <= n < 2^(i+1)
    idx_t power = 1;
    while (power <= n / 2) {
        power <<= 1;
    }
    
    idx_t r = n - power;
    
    if (r == power - 1) {
        // r = 2^i - 1, so n = 2^(i+1) - 1
        return ((seq_t)power) * 2;
    } else {
        // r < 2^i - 1, so a(n) = 2^i + a(r+1)
        return ((seq_t)power) + a(r + 1);
    }
}
