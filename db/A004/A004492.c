/*
 * A004492  Tersum n + 3.
 * https://oeis.org/A004492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = tersum(n, 3), where tersum adds in base 3 with no carries
 *            (digit-wise mod 3 addition)
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9811 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t tersum(seq_t a, seq_t b) {
    seq_t result = 0;
    seq_t power = 1;
    
    while (a > 0 || b > 0) {
        seq_t digit_sum = ((a % 3) + (b % 3)) % 3;
        result += digit_sum * power;
        a /= 3;
        b /= 3;
        power *= 3;
    }
    
    return result;
}

seq_t a(idx_t n) {
    return tersum((seq_t)n, 3);
}
