/*
 * A004183  Omit 8's from n.
 * https://oeis.org/A004183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   remove all 8 digits from n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t temp = n;
    idx_t digit_count = 0;
    idx_t digits[25];
    
    do {
        idx_t digit = temp % 10;
        if (digit != 8) {
            digits[digit_count++] = digit;
        }
        temp /= 10;
    } while (temp > 0);
    
    if (digit_count == 0) {
        return 0;
    }
    
    seq_t result = 0;
    for (idx_t i = digit_count; i > 0; i--) {
        result = result * 10 + (seq_t)digits[i - 1];
    }
    
    return result;
}
