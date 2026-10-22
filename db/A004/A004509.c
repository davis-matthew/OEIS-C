/*
 * A004509  Tersum n + 20.
 * https://oeis.org/A004509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 20), where tersum performs base-3 digit-wise
 *            addition mod 3
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t m_val = (seq_t)20;
    seq_t result = 0;
    seq_t power = 1;
    
    while (n_val > 0 || m_val > 0) {
        seq_t n_digit = n_val % 3;
        seq_t m_digit = m_val % 3;
        seq_t sum_digit = (n_digit + m_digit) % 3;
        result += sum_digit * power;
        power *= 3;
        n_val /= 3;
        m_val /= 3;
    }
    
    return result;
}
