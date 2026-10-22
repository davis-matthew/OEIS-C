/*
 * A004520  Generalized nim sum n + n in base 10.
 * https://oeis.org/A004520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + n in base 10 carryless arithmetic
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t num = (seq_t)n;
    seq_t result = 0;
    seq_t power = 1;
    
    while (num > 0) {
        seq_t digit = num % 10;
        seq_t sum_digit = (digit + digit) % 10;
        result += sum_digit * power;
        power *= 10;
        num /= 10;
    }
    
    return result;
}
