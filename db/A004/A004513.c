/*
 * A004513  Tersum n + 24.
 * https://oeis.org/A004513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 24), where tersum adds digit-wise mod 3 in base
 *            3
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    seq_t m = 24;
    seq_t n_copy = (seq_t)n;
    
    while (n_copy > 0 || m > 0) {
        seq_t digit_n = n_copy % 3;
        seq_t digit_m = m % 3;
        seq_t sum_digit = (digit_n + digit_m) % 3;
        result += sum_digit * power;
        
        n_copy /= 3;
        m /= 3;
        power *= 3;
    }
    
    return result;
}
