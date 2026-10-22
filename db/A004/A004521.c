/*
 * A004521  Generalized nim sum n + n in base 11.
 * https://oeis.org/A004521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + n in base 11 (digit-wise addition mod 11)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        seq_t result = 0;
    seq_t power = 1;
    seq_t n_val = (seq_t)n;
    
    while (n_val > 0) {
        seq_t digit = n_val % 11;
        seq_t new_digit = (2 * digit) % 11;
        result += new_digit * power;
        power *= 11;
        n_val /= 11;
    }
    
    return result;
}
