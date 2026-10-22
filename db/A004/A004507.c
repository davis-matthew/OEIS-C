/*
 * A004507  Tersum n + 18.
 * https://oeis.org/A004507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 18), where tersum adds base-3 digits of two
 *            numbers mod 3 with no carries
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t m_val = 18;
    seq_t result = 0;
    seq_t power = 1;
    
    while (n_val > 0 || m_val > 0) {
        seq_t n_digit = n_val % 3;
        seq_t m_digit = m_val % 3;
        seq_t result_digit = (n_digit + m_digit) % 3;
        result += result_digit * power;
        
        n_val /= 3;
        m_val /= 3;
        power *= 3;
    }
    
    return result;
}
