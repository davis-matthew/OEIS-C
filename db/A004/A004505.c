/*
 * A004505  Tersum n + 16.
 * https://oeis.org/A004505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n + 16), where tersum m + n is computed by writing
 *            m and n in base 3 and adding corresponding digits mod 3 with no
 *            carries
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)16;
    seq_t result = 0;
    seq_t power = 1;
    idx_t n_copy = n;
    
    while (n_copy > 0 || m > 0) {
        seq_t n_digit = (seq_t)(n_copy % 3);
        seq_t m_digit = m % 3;
        seq_t sum_digit = (n_digit + m_digit) % 3;
        result += sum_digit * power;
        power *= 3;
        n_copy /= 3;
        m /= 3;
    }
    
    return result;
}
