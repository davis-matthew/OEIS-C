/*
 * A004518  Generalized nim sum n + n in base 8.
 * https://oeis.org/A004518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   generalized nim sum n + n in base 8
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    seq_t temp_n = (seq_t)n;
    
    while (temp_n > 0) {
        seq_t digit = temp_n % 8;
        result += ((digit + digit) % 8) * power;
        power *= 8;
        temp_n /= 8;
    }
    
    return result;
}
