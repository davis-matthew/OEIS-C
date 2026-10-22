/*
 * A004512  Tersum n + 23.
 * https://oeis.org/A004512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum n + 23
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    
    uint64_t n_copy = n;
    uint64_t m_copy = 23;
    
    while (n_copy > 0 || m_copy > 0) {
        int n_digit = (int)(n_copy % 3);
        int m_digit = (int)(m_copy % 3);
        
        result += (seq_t)((n_digit + m_digit) % 3) * power;
        
        n_copy /= 3;
        m_copy /= 3;
        power *= 3;
    }
    
    return result;
}
