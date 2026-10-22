/*
 * A004495  Tersum n + 6.
 * https://oeis.org/A004495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = tersum(n, 6), where tersum is addition in base 3 without
 *            carries
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    seq_t n_val = (seq_t)n;
    seq_t constant = 6;
    
    while (n_val > 0 || constant > 0) {
        seq_t n_digit = n_val % 3;
        seq_t c_digit = constant % 3;
        seq_t sum_digit = (n_digit + c_digit) % 3;
        
        result += sum_digit * power;
        power *= 3;
        n_val /= 3;
        constant /= 3;
    }
    
    return result;
}
