/*
 * A004502  Tersum n + 13.
 * https://oeis.org/A004502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 13), where tersum adds in base 3 mod 3 with no
 *            carries
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t tersum(seq_t a, seq_t b) {
    seq_t result = 0;
    seq_t power = 1;
    
    while (a > 0 || b > 0) {
        seq_t digit_a = a % 3;
        seq_t digit_b = b % 3;
        seq_t sum_digit = (digit_a + digit_b) % 3;
        result += sum_digit * power;
        
        power *= 3;
        a /= 3;
        b /= 3;
    }
    
    return result;
}

seq_t a(idx_t n) {
    return tersum((seq_t)n, (seq_t)13);
}
