/*
 * A004517  Generalized nim sum n + n in base 7.
 * https://oeis.org/A004517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + n in base 7, digit-by-digit mod 7 with no carries
 *            (generalized nim sum)
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t multiplier = 1;
    seq_t temp = (seq_t)n;
    
    while (temp > 0) {
        seq_t digit = temp % 7;
        seq_t sum_digit = (digit + digit) % 7;
        result += sum_digit * multiplier;
        multiplier *= 7;
        temp /= 7;
    }
    
    return result;
}
