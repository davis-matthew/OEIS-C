/*
 * A004180  Omit 5's from n.
 * https://oeis.org/A004180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Remove all digits 5 from n's decimal representation
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t digits[20];
    idx_t num_digits = 0;
    idx_t temp = n;
    
    while (temp > 0) {
        digits[num_digits++] = temp % 10;
        temp /= 10;
    }
    
    // Reverse to get left-to-right order
    for (idx_t i = 0; i < num_digits / 2; i++) {
        idx_t j = num_digits - 1 - i;
        idx_t t = digits[i];
        digits[i] = digits[j];
        digits[j] = t;
    }
    
    // Combine digits, skipping 5's
    seq_t result = 0;
    for (idx_t i = 0; i < num_digits; i++) {
        if (digits[i] != 5) {
            result = result * 10 + (seq_t)digits[i];
        }
    }
    
    return result;
}
