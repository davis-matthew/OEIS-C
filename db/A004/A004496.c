/*
 * A004496  Tersum n + 7.
 * https://oeis.org/A004496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = tersum(n, 7), where tersum adds base-3 digits of n and 7
 *            modulo 3 with no carries
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t m = 7;
    seq_t result = 0;
    seq_t power = 1;
    
    while (n_val > 0 || m > 0) {
        seq_t n_digit = n_val % 3;
        seq_t m_digit = m % 3;
        seq_t digit_sum = (n_digit + m_digit) % 3;
        result += digit_sum * power;
        
        power *= 3;
        n_val /= 3;
        m /= 3;
    }
    
    return result;
}
