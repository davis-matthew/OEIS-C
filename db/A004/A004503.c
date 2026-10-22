/*
 * A004503  Tersum n + 14.
 * https://oeis.org/A004503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 14) where tersum adds in base 3 with no carries
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t digits[50] = {0};
    seq_t temp = 14;
    int digit_pos = 0;
    
    // Compute tersum: add n and 14 in base 3, digit by digit mod 3
    while (n_val > 0 || temp > 0) {
        seq_t d_n = n_val % 3;
        seq_t d_14 = temp % 3;
        digits[digit_pos] = (d_n + d_14) % 3;
        
        n_val /= 3;
        temp /= 3;
        digit_pos++;
    }
    
    // Convert result back from base 3 to decimal
    seq_t result = 0;
    seq_t power = 1;
    for (int i = 0; i < digit_pos; i++) {
        result += digits[i] * power;
        power *= 3;
    }
    
    return result;
}
