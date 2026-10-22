/*
 * A004511  Tersum n + 22.
 * https://oeis.org/A004511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 22); base-3 digit-by-digit addition mod 3 with
 *            no carries
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    seq_t n_val = (seq_t)n;
    seq_t m = 22;
    
    while (n_val > 0 || m > 0) {
        seq_t n_digit = n_val % 3;
        seq_t m_digit = m % 3;
        seq_t sum = (n_digit + m_digit) % 3;
        result += sum * power;
        power *= 3;
        n_val /= 3;
        m /= 3;
    }
    
    return result;
}
