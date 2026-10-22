/*
 * A000593  Sum of odd divisors of n.
 * https://oeis.org/A000593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, easy, nice, mult
 * Formula:   a(n) = sum of odd divisors of n
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }
    
    seq_t result = 0;
    for (idx_t d = 1; d <= m; ++d) {
        if (m % d == 0) {
            result += (seq_t)d;
            idx_t complement = m / d;
            if (d < complement) {
                result += (seq_t)complement;
            }
        }
        if (d > m / d) break;
    }
    return result;
}
