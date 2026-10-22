/*
 * A004508  Tersum n + 19.
 * https://oeis.org/A004508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 19), base-3 digit addition mod 3 with no
 *            carries
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
    seq_t constant = 19;
    
    while (n_val > 0 || constant > 0) {
        seq_t digit_n = n_val % 3;
        seq_t digit_c = constant % 3;
        seq_t digit_sum = (digit_n + digit_c) % 3;
        
        result += digit_sum * power;
        
        power *= 3;
        n_val /= 3;
        constant /= 3;
    }
    
    return result;
}
