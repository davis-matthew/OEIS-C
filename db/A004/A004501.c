/*
 * A004501  Tersum n + 12.
 * https://oeis.org/A004501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = tersum(n + 12), where tersum addition is base-3
 *            digit-wise addition modulo 3 with no carries
 * Range:     u8 n <= 243, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t m = 12;
    seq_t result = 0;
    seq_t power = 1;
    
    while (n_val > 0 || m > 0) {
        seq_t n_digit = n_val % 3;
        seq_t m_digit = m % 3;
        seq_t sum_digit = (n_digit + m_digit) % 3;
        result += sum_digit * power;
        
        n_val /= 3;
        m /= 3;
        power *= 3;
    }
    
    return result;
}
